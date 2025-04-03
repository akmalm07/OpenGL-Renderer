#pragma once

#include "config.h"


#include "tools/include/stride.h"

#include "tools/include/camera.h"
#include "tools/include/timer.h"
#include "tools/include/thread.h"
#include "glInit/include/program.h"
#include "glUtil/include/mesh.h"
#include "glUtil/include/uniform_buffer.h"


#include "tools/include/window.h"


#include <memory>


class BaseEngine 
{

public:

	BaseEngine();

	BaseEngine(int width, int height, bool orthoOrPerpective, bool debug);

	BaseEngine(tools::Window& window, bool debug);
	
	void input_meshes(std::vector<glUtil::Mesh>& meshes);
	
	void input_uniform_buff(std::vector<glUtil::UniformBuffer>& meshes);

	void render();

	virtual void draw() const = 0;
	
	virtual void game_logic(const double& deltaTime) = 0;
	
	virtual void update_sets() = 0;
	
	virtual void camera_logic() = 0;

	void update_FPS();

	virtual ~BaseEngine(); 

protected:

	tools::Window _window;
	
	tools::Camera _camera;

	std::vector<glUtil::Mesh> _meshes;
	
	std::vector<glUtil::UniformBuffer> _uniformBuffs;

	double _deltaTime = 0.0;

	GLProgram _program;



	//Matrices

	bool _debugMode = false;

	alignas(16) struct Matrices
	{
		glm::mat4 _modelMat;
		glm::mat4 _viewMat;
		glm::mat4 _projMat;
	} _MVPMats;



	bool _firstCall = true;

	protected:

	void build_glfw_window();

	void build_glfw_window(int width, int height, bool orthoOrPerpective);

	void camera_init(bool orthoOrPerpective);

	void read_json_files();

	void make_pipeline();

	void make_uniform_objects();

	void make_framebuffer();



};
