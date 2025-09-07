#pragma once

#include "config.h"

#include "glUtil\include\mesh.h"
#include "glUtil\include\texture.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>


namespace tools
{

	class Scene
	{
	public:

		Scene();

		Scene(Scene&& other) noexcept;

		Scene& operator=(Scene&& other) noexcept;

		void init(glInit::GLProgram& program, const std::filesystem::path& modelPath);

		void process_node(glInit::GLProgram& program, aiNode* node, const aiScene* scene);
		
		void process_mesh(glInit::GLProgram& program, aiMesh* mesh, const aiScene* scene);

		std::vector<glUtil::Texture> load_material_textures(glInit::GLProgram& program, aiMaterial* mat, aiTextureType type, std::string typeName);



		~Scene();

	private:

		bool _debug;
		std::filesystem::path _directory;
		std::vector<glUtil::Texture> _texturesloaded;
		std::vector<glUtil::Mesh> _meshes;

	};

}