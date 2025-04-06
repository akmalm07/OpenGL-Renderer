#pragma once

#include "config.h"
#include "tools/include/scene.h"


class World
{
public:
	World();
	World(World&& other) noexcept;
	World& operator=(World&& other) noexcept;
	~World();

	void add_scene(std::shared_ptr<tools::Scene> scene);

	void update_scene() const;

	World(const World&) = delete;
	World& operator=(const World&) = delete;

private:
	std::shared_ptr<tools::Scene> scenes;

	glm::vec3 ambientLight; 

	void initalize();
};