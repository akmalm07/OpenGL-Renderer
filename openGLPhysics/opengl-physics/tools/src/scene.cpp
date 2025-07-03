#include "headers.h"
//
//#include "tools\include\scene.h"
//
//
//namespace tools 
//{
//
//
//    Scene::Scene() {}
//
//
//	Scene::Scene(Scene&& other) noexcept = default;
//
//
//    Scene& Scene::operator=(Scene&& other) noexcept = default;
//
//    Scene::~Scene() = default;
//
//
//    void Scene::init(const std::filesystem::path& modelPath)
//    {
//        Assimp::Importer importer;
//        const aiScene* scene = importer.ReadFile(modelPath.string(),
//            aiProcess_Triangulate |
//            aiProcess_FlipUVs |
//            aiProcess_GenNormals);
//
//
//        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//        {
//            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
//            return;
//        }
//
//
//        process_node(scene->mRootNode, scene);
//    }
//
//
//    void Scene::process_node(aiNode* node, const aiScene* scene)
//    {
//        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
//        {
//            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//            process_mesh(mesh, scene);
//        }
//        for (unsigned int i = 0; i < node->mNumChildren; ++i)
//        {
//            process_node(node->mChildren[i], scene);
//        }
//    }
//
//
//    void Scene::process_mesh(aiMesh* mesh, const aiScene* scene)
//    {
//        std::vector<float> vertices;
//        std::vector<unsigned int> indices;
//        std::vector<glUtil::Texture> textures;
//
//
//        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
//        {
//            // Positions
//            vertices.push_back(mesh->mVertices[i].x);
//            vertices.push_back(mesh->mVertices[i].y);
//            vertices.push_back(mesh->mVertices[i].z);
//
//
//            // Normals (if available)
//            if (mesh->mNormals)
//            {
//                vertices.push_back(mesh->mNormals[i].x);
//                vertices.push_back(mesh->mNormals[i].y);
//                vertices.push_back(mesh->mNormals[i].z);
//            }
//            else
//            {
//                vertices.push_back(0.0f);
//                vertices.push_back(0.0f);
//                vertices.push_back(0.0f);
//            }
//
//
//            if (mesh->mTextureCoords[0]) 
//            {
//                vertices.push_back(mesh->mTextureCoords[0][i].x);
//                vertices.push_back(mesh->mTextureCoords[0][i].y);
//            }
//            else
//            {
//                vertices.push_back(0.0f);
//                vertices.push_back(0.0f);
//            }
//        }
//
//
//        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
//        {
//            aiFace face = mesh->mFaces[i];
//            for (unsigned int j = 0; j < face.mNumIndices; ++j)
//            {
//                indices.push_back(face.mIndices[j]);
//            }
//        }
//
//
//        // Materials (if available)
//        if (mesh->mMaterialIndex >= 0)
//        {
//            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//            // Load diffuse textures
//            std::vector<glUtil::Texture> diffuseMaps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//            // Load specular textures
//            std::vector<glUtil::Texture> specularMaps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
//            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//            // Load normal textures
//            std::vector<glUtil::Texture> normalMaps = load_material_textures(material, aiTextureType_NORMALS, "texture_normal");
//            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
//            // Load height maps
//            std::vector<glUtil::Texture> heightMaps = load_material_textures(material, aiTextureType_HEIGHT, "texture_height");
//            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
//        }
//
//        if (_debug)
//        {
//            std::cout << "Mesh loaded with " << vertices.size() / 9 << " vertices and " << indices.size() / 3 << " triangles." << std::endl;
//        }
//    }
//
//
//    std::vector<glUtil::Texture> Scene::load_material_textures(aiMaterial* mat, aiTextureType type, std::string typeName)
//    {
//        std::vector<glUtil::Texture> textures;
//        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
//        {
//            aiString str;
//            mat->GetTexture(type, i, &str);
//            bool skip = false;
//            for (unsigned int j = 0; j < _texturesloaded.size(); ++j)
//            {
//                if (std::strcmp(_texturesloaded[j].get_path().string().c_str(), str.C_Str()) == 0)
//                {
//                    textures.push_back(_texturesloaded[j]);
//                    skip = true;
//                    break;
//                }
//            }
//            if (!skip)
//            { 
//                glUtil::Texture texture(_directory.string() + "\\" + str.C_Str()); // CRITIAL lvl 5
//                textures.push_back(texture);
//                _texturesloaded.push_back(texture); 
//            }
//        }
//        return textures;
//    }
//
//}