#include "headers.h"
// tools/src/scene.cpp
#include "tools/include/scene.h"

#include "glInit/include/program.h"

#include <stdexcept>
#include <unordered_set>
#include <iostream> // optional for debug logs

using namespace tools;

namespace
{
    // --- Adapt this to your actual glUtil::Texture API in one place ---
    static glUtil::Texture make_texture_from_file(glInit::GLProgram& program, const std::filesystem::path& full_path)
    {
        // Example patterns (pick the one matching your Texture class and remove others):
        //
        // 1) If your Texture looks like LearnOpenGL's { id, type, path } and you have
        //    a loader elsewhere, you might just store path/type here:
        // glUtil::Texture tex;
        // tex.type = type_name;
        // tex.path = full_path.string();
        // tex.id   = LoadTexture(full_path.string().c_str()); // <- your loader
        // return tex;
        //
        // 2) If you have a constructor taking (path, type):
        // return glUtil::Texture(full_path.string(), type_name);
        //
        // 3) If you have a static loader:
        // return glUtil::Texture::FromFile(full_path.string(), type_name);
        //
        // Fallback: try a minimal common-case ctor (adjust as needed):
        return glUtil::Texture(program, full_path);
    }

    // Normalize an Assimp texture path (handles embedded './' etc.)
    static std::filesystem::path normalize_path(const std::filesystem::path& base_dir,
        const aiString& ai_path)
    {
        std::string p = ai_path.C_Str();
        // Assimp sometimes gives backslashes; normalize to your FS style.
        for (char& c : p)
        {
            if (c == '\\') c = '/';
        }
        return (base_dir / p).lexically_normal();
    }
}

Scene::Scene()
    : _debug(false)
{
}

Scene::Scene(Scene&& other) noexcept
    : _debug(other._debug)
    , _directory(std::move(other._directory))
    , _texturesloaded(std::move(other._texturesloaded))
{
}

Scene& Scene::operator=(Scene&& other) noexcept
{
    if (this != &other)
    {
        _debug = other._debug;
        _directory = std::move(other._directory);
        _texturesloaded = std::move(other._texturesloaded);
    }
    return *this;
}

void Scene::init(glInit::GLProgram& program, const std::filesystem::path& modelPath)
{
    Assimp::Importer importer;

    // Tweak flags as needed for your pipeline
    const unsigned int kFlags =
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_LimitBoneWeights |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SortByPType |
        aiProcess_GenUVCoords |
        aiProcess_OptimizeMeshes |
        aiProcess_ValidateDataStructure;

    const aiScene* scene = importer.ReadFile(modelPath.string(), kFlags);

    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
    {
        throw std::runtime_error(std::string("Assimp error: ") + importer.GetErrorString());
    }

    _directory = modelPath.parent_path();
    _texturesloaded.clear();

    if (_debug)
    {
        std::cout << "[Scene] Loading: " << modelPath << "\n";
        std::cout << "  meshes: " << scene->mNumMeshes
            << ", materials: " << scene->mNumMaterials
            << ", animations: " << scene->mNumAnimations << "\n";
    }

    process_node(program, scene->mRootNode, scene);
}

void Scene::process_node(glInit::GLProgram& program, aiNode* node, const aiScene* scene)
{
    // Process meshes attached to this node
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        process_mesh(program, mesh, scene);
    }

    // Recurse into children
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        process_node(program, node->mChildren[i], scene);
    }
}

void Scene::process_mesh(glInit::GLProgram& program, aiMesh* mesh, const aiScene* scene)
{
    if (_debug)
    {
        std::cout << "[Scene] process_mesh: "
            << (mesh->mName.C_Str() ? mesh->mName.C_Str() : "<unnamed>")
            << " | vtx: " << mesh->mNumVertices
            << " | faces: " << mesh->mNumFaces << "\n";
    }

    struct Vert
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
	};

    std::vector<Vert> vertices;
    std::vector<unsigned int> indices;
    std::vector<glUtil::Texture> textures;

    // 1. Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vert vertex;
        glm::vec3 vector;

        // Position
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // Normal
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        if (mesh->mTextureCoords[0]) 
        {
            glm::vec2 tex;
            tex.x = mesh->mTextureCoords[0][i].x;
            tex.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = tex;
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        auto diffuseMaps = load_material_textures(program, material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        auto specularMaps = load_material_textures(program, material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        auto normalMaps = load_material_textures(program, material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        auto heightMaps = load_material_textures(program, material, aiTextureType_HEIGHT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    //_meshes.emplace_back(vertices, indices); In construction
}


std::vector<glUtil::Texture>
Scene::load_material_textures(glInit::GLProgram& program, aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<glUtil::Texture> textures;

    const unsigned int count = mat->GetTextureCount(type);
    if (_debug && count > 0)
    {
        std::cout << "  [Scene] Found " << count << " textures for type " << typeName << "\n";
    }

    for (unsigned int i = 0; i < count; ++i)
    {
        aiString str;
        if (mat->GetTexture(type, i, &str) != AI_SUCCESS)
            continue;

        const std::filesystem::path full_path = normalize_path(_directory, str);

        // De-dup: if already loaded, reuse it
        bool already_loaded = false;
        for (const auto& loaded : _texturesloaded)
        {
            // Adjust if your Texture stores its path differently
            if (loaded.get_path() == full_path.string())
            {
                textures.push_back(loaded);
                already_loaded = true;
                break;
            }
        }

        if (!already_loaded)
        {
            glUtil::Texture tex = make_texture_from_file(program, full_path);
            textures.push_back(tex);
            _texturesloaded.push_back(tex);
        }
    }

    return textures;
}

Scene::~Scene() = default;
