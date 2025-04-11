#pragma once

#include "config.h"

#include "glUtil\include\mesh.h"

namespace tools
{
	class Scene
	{
	public:

		Scene();

		Scene(Scene&& other) noexcept;

		Scene& operator=(Scene&& other) noexcept;

		~Scene();


	private:

		void initalize();
	};

}