#pragma once

#include "config.h"

#include "tools\include\base_engine.h"

#include "tools\include\scene.h"
#include "glUtil\include\mesh.h"


class Engine : public BaseEngine
{
public:

	Engine();

	Engine(int width, int height, bool orthoOrperspective, bool debug);

	Engine(const tools::Window& window, bool orthoOrperspective, bool debug);

	Engine(GLFWwindow* glfwWindow, bool orthoOrperspective, bool debug);

	void load_meshes(std::vector<Mesh>& meshes) const;
	
	void load_meshes(std::vector<glType::Vertex>& meshes, std::vector<glType::Index>& indices) const;

	void load_mesh(const Mesh& mesh) const;

	void game_logic(const double& deltaTime) override;

	void camera_logic() override;

	void draw() const override;  

	~Engine() override;


private:

	mutable std::vector<std::function<void()>> _pushConstCalls;  

	std::vector<Mesh> _mesh;

private:
};


//void load_scene(std::unique_ptr<Scene> scene);
//void draw_scene(vk::CommandBuffer& cmdBuffer) const;