#include "headers.h"



#include "tools/include/base_engine.h"

#include "tools/include/camera.h"
#include "tools/include/camera_bundle.h"

#include "glInit/include/config.h"


BaseEngine::BaseEngine()
{
	
	_debugMode = true;

	if (_debugMode) {
		std::cout << "Making a graphics BaseEngine\n";
	}

	camera_init(true);

	build_glfw_window(800, 800, true);

	read_json_files();

	make_framebuffer();

	make_pipeline();

}


BaseEngine::BaseEngine(int width, int height, bool orthoOrPerpective, bool debug) 
{

	_debugMode = debug;

	if (_debugMode) {
		std::cout << "Making a graphics BaseEngine\n"; 
	}

	camera_init(orthoOrPerpective);

	build_glfw_window(width, height, orthoOrPerpective);

	read_json_files();

	make_framebuffer();

	make_pipeline();

}

BaseEngine::BaseEngine(tools::Window& glfwWindow, bool debug)
{

	_debugMode = debug;

	_window = std::move(glfwWindow);
	_window.create_window(false, _window.is_ortho());


	_debugMode = debug;


	if (_debugMode) {
		std::cout << "Making a graphics BaseEngine\n";
	}

	camera_init(_window.is_ortho());

	build_glfw_window();

	read_json_files();

	make_framebuffer();

	make_pipeline();

}


void BaseEngine::input_meshes(std::vector<glUtil::Mesh>& meshes)
{
	_meshes = std::move(meshes);

	for (const auto& mesh : _meshes)
	{
		if (mesh.is_init())
		{
			throw std::runtime_error("Mesh not initialized");
		}
	}
}

void BaseEngine::input_uniform_buff(std::vector<glUtil::UniformBuffer>& meshes)
{
	//_uniformBuffs = std::move(meshes);
	//for (const auto& mesh : _uniformBuffs)
	//{
	//	if (mesh.is_init())
	//	{
	//		throw std::runtime_error("Mesh not initialized");
	//	}
	//}
}



void BaseEngine::update_FPS()
{
	static double time = 0;
	static uint32_t frameCount = 0;

	time += _deltaTime;
	frameCount++;
	
	if (time >= 1.0) 
	{
		std::stringstream title;
		title << _window.get_name() << " - FPS: " << frameCount;
		_window.set_name(title.str().c_str());

		frameCount = 0;
		time = 0;
	}
}


void BaseEngine::build_glfw_window() 
{ 
	_window = std::move(tools::Window(800, 800, "Untitled Window"));
	_window.create_window(true, false);
}


void BaseEngine::build_glfw_window(int width, int height, bool orthoOrPerpective)
{ 
	_window = std::move(tools::Window(width, height, "Vulkan"));
	_window.create_window(false , orthoOrPerpective);
}


void BaseEngine::read_json_files() 
{
	//JsonReader decriptorSet(_jsonDescriptorSetsFilePath);

	//std::vector<vkInit::DescriptorSetBindings> descriptorSets;

	//for (const auto& set : decriptorSet["descriptor_sets"])
	//{
	//	vkInit::DescriptorSetBindings descriptorSet;
	//	for (const auto& binding : set["bindings"])
	//	{
	//		if (binding["stage_flags"].is_array())
	//		{
	//			std::vector<vkUtil::ShaderStage> stages;
	//			for (const auto& stage : binding["stage_flags"])
	//			{
	//				stages.emplace_back(vkUtil::to_shader_stage(stage.get<std::string_view>()));
	//			}
	//			descriptorSet.emplace_back
	//			(
	//				vkInit::to_descriptor_type(binding["descriptor_type"]),
	//				stages,
	//				binding["descriptor_count"].get<uint32_t>(),
	//				binding["binding"].get<uint32_t>()
	//			);
	//		}
	//		else
	//		{
	//			descriptorSet.emplace_back
	//			(
	//				vkInit::to_descriptor_type(binding["descriptor_type"]),
	//				vkUtil::to_shader_stage(binding["stage_flags"]),
	//				binding["descriptor_count"].get<uint32_t>(),
	//				binding["binding"].get<uint32_t>()

	//			);
	//		}

	//	}
	//	descriptorSets.push_back(std::move(descriptorSet));
	//}

	//std::vector<vkInit::DescriptorSetBindings> layoutBindings;
	//for (const auto& layouts : decriptorSet["layouts"])
	//{
	//	vkInit::DescriptorSetBindings layoutBinding;
	//	for (const auto& layout : layouts["bindings"])
	//	{
	//		if (layout["stage_flags"].is_array())
	//		{
	//			std::vector<vkUtil::ShaderStage> stages;
	//			for (const auto& stage : layout["stage_flags"])
	//			{
	//				stages.emplace_back(vkUtil::to_shader_stage(stage.get<std::string_view>()));
	//			}
	//			layoutBinding.emplace_back
	//			(
	//				vkInit::to_descriptor_type(layout["descriptor_type"]),
	//				stages,
	//				layout["descriptor_count"].get<uint32_t>(),
	//				layout["binding"].get<uint32_t>()
	//			);
	//		}
	//		else
	//		{
	//			layoutBinding.emplace_back
	//			(
	//				vkInit::to_descriptor_type(layout["descriptor_type"]),
	//				vkUtil::to_shader_stage(layout["stage_flags"]),
	//				layout["descriptor_count"].get<uint32_t>(),
	//				layout["binding"].get<uint32_t>()
	//			);
	//		}
	//	}			
	//	layoutBindings.push_back(std::move(layoutBinding));
	//}
	//

	//std::vector<vkUtil::BufferInput> bufferInfo;
	//for (const auto& info : decriptorSet["descriptor_buffers"])
	//{
	//	bufferInfo.emplace_back
	//	(
	//		info["size_max"].get<uint32_t>(),
	//		vkInit::to_buffer_type(info["type"].get<std::string_view>())
	//	);
	//}

	//tools::DescriptorSetRegistry::get_instance().intialize(descriptorSets, layoutBindings, bufferInfo);


	//JsonReader pushConst(_jsonPushCosntantsFilePath);

	//std::vector<vkType::PushConst> pushConsts;

	//uint32_t offset = 0;
	//for (const auto& push : pushConst["push_consts"])
	//{
	//	pushConsts.emplace_back
	//	(
	//		push["size"].get<uint32_t>(),
	//		offset,
	//		vkUtil::enum_to_vk(vkUtil::to_shader_stage(push["shader"].get<std::string_view>()))
	//		
	//	);

	//	offset += push["size"].get<uint32_t>();
	//}

	//tools::PushConstRegistery::get_instance().initalize(pushConsts);
}


void BaseEngine::make_pipeline()
{
	Config& config = Config::instance();
	_program.create_shaders_from_files(config.get_vert_shader_path(), config.get_frag_shader_path());
	
}

void BaseEngine::make_uniform_objects()
{

}



void BaseEngine::make_framebuffer()
{
	
}


void BaseEngine::camera_init(bool orthoOrPerpective)
{
	if (orthoOrPerpective)
	{
		tools::CameraBundleOrthographic bundle; 
		bundle.bottom = _window.get_bottom_ortho();
		bundle.left = _window.get_left_ortho();
		bundle.right = _window.get_right_ortho();
		bundle.top = _window.get_top_ortho();
		bundle.nearZ = 0.001f;
		bundle.farZ = 1000.0f;
		bundle.speed = 100.0f;
		bundle.turnSpeed = 100.0f;
		bundle.position = glm::vec3(0.0f, 0.0f, 0.0f);
		bundle.startPYR = glm::vec3(0.0f, 0.0f, 0.0f);
		bundle.front = glm::vec3(0.0f, 0.0f, -1.0f);
		//bundle.direction = tools::Direction::Forward;
		_camera = std::move(tools::Camera(bundle));
	}
	else
	{
		tools::CameraBundlePerspective bundle;
		bundle.fov = 60.0f;         
		bundle.aspectRatio = _window.get_aspect_ratio();
		bundle.nearZ = 0.1f;         
		bundle.farZ = 100.0f;        
		bundle.speed = 0.0f;         
		bundle.turnSpeed = 0.0f;     
		bundle.position = glm::vec3(0.0f, 0.0f, 5.0f);
		bundle.startPYR = glm::vec3(0.0f, 0.0f, 0.0f); 
		bundle.front = glm::vec3(0.0f, 0.0f, -1.0f);
		
		_camera = std::move(tools::Camera(bundle)); 
	}
	_camera.set_commands_to_window(_window);

	

	_MVPMats._viewMat = _camera.get_view();
	_MVPMats._projMat = _camera.get_projection();
	_MVPMats._modelMat = glm::mat4(1.0f);

}




void BaseEngine::render()
{


	
}


BaseEngine::~BaseEngine()
{
	_window.clear_window();
}

