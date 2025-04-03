#pragma once

#include "tools/include/window.h"
#include "glUtil/include/mesh.h"
#include "glInit/include/program.h"
#include "glUtil/include/uniform_buffer.h"
#include "tools/include/camera.h"
#include "config.h"


namespace Renderer
{
	void Run()
	{
		using namespace tools;
		using namespace glUtil;

		Window window(800, 600, "OpenGL");
		window.create_window(false, true);
		
		CameraBundleOrthographic cameraBundleOrtho = {};
		cameraBundleOrtho.position = glm::vec3(0.0f, 0.0f, 1.0f); 
		cameraBundleOrtho.front = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraBundleOrtho.worldUp = glm::vec3(0.0f, 1.0f, 0.0f); 
		cameraBundleOrtho.left = window.get_left_ortho();
		cameraBundleOrtho.right = window.get_right_ortho();
		cameraBundleOrtho.bottom = window.get_bottom_ortho();
		cameraBundleOrtho.top = window.get_top_ortho();
		cameraBundleOrtho.nearZ = 0.1f; 
		cameraBundleOrtho.farZ = 1000.0f; 
		cameraBundleOrtho.speed = 1.0f; 

		Camera camera(cameraBundleOrtho);

		Mesh mesh;




		MeshBundle bundle;


		std::array<float, 24> vertices = {
			0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f,
			-0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		std::array<unsigned int, 6> indices = {
			0, 1, 2,  
			2, 3, 0  
		};
		
		GLProgram program;

		program.create_shaders_from_files("shaders/shader.vert", "shaders/shader.frag");


		ArrayBufferLayout layout1;
		layout1.location = 0;
		layout1.stride = Stride::STRIDE_3D;
		layout1.type = StrideType::POS;
		ArrayBufferLayout layout2;
		layout2.location = 1;
		layout2.stride = Stride::STRIDE_3D;
		layout2.type = StrideType::COL;

		bundle.indexCount = indices.size();
		bundle.vertexCount = vertices.size();
		bundle.pVertices = vertices.data();
		bundle.pIndices = indices.data();
		bundle.fullStride = FullStride::STRIDE_6D;
		bundle.pLayout1 = &layout1;
		bundle.pLayout2 = &layout2;


		mesh.init(bundle, true);

		window.set_escape_button(Keys::Esc);

		camera.set_commands_to_window(window); 
		
		struct Matrix
		{
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
		}matrix;

		matrix.projection = camera.get_projection();
		matrix.view = camera.get_view(); 
		
		bool change = false;

		tools::KeyUsageRegistry& keys = tools::KeyUsageRegistry::get_instance();


		for (const auto& [key, mod] : keys.keys_in_use())
		{
			std::function<bool()> func = [&matrix, &camera, val = window.FindKeyComb(key)]() -> bool
				{
					val->change_parameters(0.1);
					matrix.view = camera.get_view();
					return true;
				};

			window.SetFuncParamUpdaterKeys(key, std::move(func), mod);
		}

		window.SetMouseChangeUpdater([&matrix, &camera, mouseMove = window.GetMouseMove(), &window]() -> bool
			{
				mouseMove->change_parameters(0.1, window.GetMouseChangeYf(), window.GetMouseChangeXf());
				std::cout << "Change X: " << window.GetMouseChangeXf() << " Change Y: " << window.GetMouseChangeYf() << std::endl;
				matrix.view = camera.get_view();
				return true;
			}
		);

		window.set_cursor_locked(); 

		glUtil::UniformBuffer uniformBuffer(0, matrix, sizeof(glm::mat4), 3, false);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		while (!window.get_should_close())
		{
			uniformBuffer.update_data(matrix);
			
			uniformBuffer.bind();


			glClearColor(1.0f, 0.5f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			program.use_shaders();
			
			mesh.render();
			
			window.poll_events();
			window.swap_buffers();	
			
			uniformBuffer.unbind();

		}

	}
}