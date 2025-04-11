#include "headers.h"

#include "tools\include\scene.h"

namespace tools
{
	Scene::Scene()
	{
		initalize();
	}


	Scene::Scene(Scene&& other) noexcept = default;

	Scene& Scene::operator=(Scene&& other) noexcept
	{
		if (this != &other)
		{

		}
		return *this;
	}

	Scene::~Scene() = default;



	void Scene::initalize()
	{
	}

}
