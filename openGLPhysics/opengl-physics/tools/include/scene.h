#pragma once

#include "config.h"

#include "glUtil/include/mesh.h"
#include "glUtil/include/uniform_buffer.h"

namespace tools
{ 

	class Scene
	{
	public:

		Scene();


		Scene(Scene&& other) noexcept;

		Scene& operator=(Scene&& other) noexcept;

		std::vector <glUtil::Mesh> get_triangles();

		void update_meshes() const;

		~Scene();


	private:
		std::vector<glUtil::Mesh> meshes;

		glUtil::UniformBuffer uniformBuffer; 

		void initalize();
	};


}

