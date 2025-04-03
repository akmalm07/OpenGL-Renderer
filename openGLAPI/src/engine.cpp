#include "headers.h"
//
//#include "tools\include\engine.h"
//#include "tools\include\descriptor_set_registry.h"
//#include "tools\include\const_push_registry.h"
//#include "tools\include\key_usage_registry.h"
//
//
//Engine::Engine() : 
//	BaseEngine()
//{
//	camera_logic();
//}
//
//
//Engine::Engine(GLFWwindow* glfwWindow, bool orthoOrperspective, bool debug) :
//	BaseEngine(glfwWindow, orthoOrperspective, debug), _scene(nullptr)
//{
//	camera_logic();
//}
//
//
//Engine::Engine(tools::WindowT& window, bool orthoOrperspective, bool debug) :
//	BaseEngine(window, orthoOrperspective, debug), _scene(nullptr)
//{
//	camera_logic();
//}
//
//
//Engine::Engine(int width, int height, bool orthoOrperspective, bool debug) :
//	BaseEngine(width, height, orthoOrperspective, debug), _scene(nullptr)
//{
//	camera_logic();
//}
//
//
//void Engine::load_mesh(MeshT& mesh) const
//{
//	std::vector<vkType::Vertex> vertices;
//
//	std::vector<vkType::Index> indices;
//
//	const auto& verts = mesh.get_vertices_raw();
//
//	vertices.insert(vertices.end(), MOVE_ITR(verts.begin()), MOVE_ITR(verts.end()));
//
//	DEBUG_ITERATOR(verts);
//
//	vkUtil::BufferInitInput bufferInfo = {};
//
//	bufferInfo.logicalDevice = _vkLogicalDevice;
//
//	bufferInfo.physicalDevice = _vkPhysicalDevice;
//
//	_vertexBuffer = std::make_unique<vkType::VertexBuffer>(bufferInfo);
//	_vertexBuffer->initalize(vertices, _debugMode);
//
//	const auto& inds = mesh.get_indices();
//	if (!inds.empty()) 
//	{
//		indices.insert(indices.end(), MOVE_ITR(inds.begin()), MOVE_ITR(inds.end())); 
//		_indexBuffer = std::make_unique<IndexBuffer>(bufferInfo);
//		_indexBuffer->initalize(indices, _debugMode);
//	}
//}
//
//
//
//void Engine::game_logic(const double& deltaTime)
//{
//
//	
//}
//
//
//
//void Engine::load_meshes(std::vector<MeshT>& meshes) const
//{
//	size_t indicesSize = 0, verticesSize = 0;
//
//	for (const auto& mesh : meshes)
//	{
//		verticesSize += mesh.get_vert_count();
//		indicesSize += mesh.ind_size();
//	}
//
//	std::vector<vkType::Vertex> vertices;
//	vertices.reserve(verticesSize);
//
//	std::vector<vkType::Index> indices;
//	indices.reserve(indicesSize);
//
//	for (auto& mesh : meshes)
//	{
//		const auto& verts = mesh.get_vertices_raw();
//		vertices.insert(vertices.end(), MOVE_ITR(verts.begin()), MOVE_ITR(verts.end()));
//		DEBUG_ITERATOR(verts);
//		if (mesh.get_indices().empty())
//		{
//			continue;
//		}
//		const auto& inds = mesh.get_indices();
//		indices.insert(indices.end(), MOVE_ITR(inds.begin()), MOVE_ITR(inds.end()));
//	}
//
//
//
//	vkUtil::BufferInitInput bufferInfo = {};
//
//	bufferInfo.logicalDevice = _vkLogicalDevice;
//
//	bufferInfo.physicalDevice = _vkPhysicalDevice;
//
//	_vertexBuffer = std::make_unique<vkType::VertexBuffer>(bufferInfo);
//
//	_vertexBuffer->initalize(vertices, _debugMode);
//
//	DEBUG_ITERATOR(vertices);
//
//
//	if (!indices.empty())
//	{
//		_indexBuffer = std::make_unique<IndexBuffer>(bufferInfo);
//		_indexBuffer->initalize(indices, _debugMode);
//	}
//}
//
//void Engine::load_meshes(std::vector<vkType::Vertex>& meshes, std::vector<vkType::Index>& indices) const
//{
//
//	vkUtil::BufferInitInput bufferInfo = {};
//
//	bufferInfo.logicalDevice = _vkLogicalDevice;
//
//	bufferInfo.physicalDevice = _vkPhysicalDevice;
//
//	_vertexBuffer = std::make_unique<vkType::VertexBuffer>(bufferInfo);
//
//	_vertexBuffer->initalize(meshes, _debugMode);
//
//	DEBUG_ITERATOR(meshes); 
//
//
//	if (!indices.empty()) 
//	{
//		_indexBuffer = std::make_unique<IndexBuffer>(bufferInfo); 
//		_indexBuffer->initalize(indices, _debugMode); 
//	}
//
//}
//
//
//
//
//void Engine::load_scene(std::unique_ptr<SceneT> scene)
//{
//	if (scene)
//	{
//		_scene = std::move(scene);	
//	}
//}
//
//
//
//void Engine::draw_scene(vk::CommandBuffer& cmdBuffer) const
//{
//	if (_scene)
//	{
//		_scene->update_sets(cmdBuffer);
//	}
//}
//
//
//void Engine::draw(vk::CommandBuffer& commandBuffer) const
//{
//
//	draw_scene(commandBuffer);
//	
//	bool hasVertBuffer = _vertexBuffer != nullptr; 
//	bool hasIndBuffer = (_indexBuffer != nullptr) && (hasVertBuffer); 
//
//	if (hasVertBuffer)
//	{
//		std::vector<vk::DeviceSize> vertOffset = { 0 };
//		_vertexBuffer->bind_buffer(commandBuffer, vertOffset);
//	}
//	if (hasIndBuffer) 
//	{
//		_indexBuffer->bind_buffer(commandBuffer, 0);
//	}
//
//	call_push_consts();
//
//	if (hasIndBuffer && hasVertBuffer)
//	{
//		uint32_t indexCount = UINT32(_indexBuffer->index_count()); 
//		commandBuffer.drawIndexed(indexCount, 1, 0, 0, 0); 
//	}
//	else if (hasVertBuffer)
//	{
//		uint32_t vertCount = UINT32(_vertexBuffer->vertex_count());
//		commandBuffer.draw(vertCount, 1, 0, 0);
//	}
//
//
//}
//
//void Engine::update_sets()
//{
//	tools::DescriptorSetRegistry& reg = tools::DescriptorSetRegistry::get_instance();
//
//	std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
//
//
//	for (const auto& [i, set] : _vkDescriptorSets.updated | std::views::enumerate)
//	{
//		for (const auto& [j, updated] : set | std::views::enumerate)
//		{
//			if (updated.status)
//			{
//				_vkDescriptorSets.updated[i][j].status = false;
//
//				if (updated.is_id(DescSetID::UniformBuff, 0))
//				{
//					_vkDescriptorSets.updated[SIZET(Sets::Set1)][SIZET(Set1::Binding1)].status = false;
//			
//					_MVPMats._viewMat = _camera.get_view();
//					update_buffer(DescSetBuff::Buffer1, _MVPMats);
//
//					writeDescriptorSets.push_back(
//						write_descriptor_set(
//							get_buffer_info(DescSetBuff::Buffer1),
//							_vkDescriptorSets.sets[SIZET(Sets::Set1)], 
//							vk::DescriptorType::eUniformBuffer, 
//							0)
//					);
//				}
//
//			}
//		}
//	}
//	
//	if (!writeDescriptorSets.empty())
//	{
//		_vkLogicalDevice.waitForFences(1, &_vkUpdateSetsFence, VK_TRUE, UINT64_MAX);
//		_vkLogicalDevice.resetFences(1, &_vkUpdateSetsFence);
//
//		_vkLogicalDevice.updateDescriptorSets(writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
//	}
//	_shouldUpdateDescSets = false;
//}
//
//
//
//void Engine::call_push_consts() const
//{
//	for (const auto& pushConst : _pushConstCalls)
//	{
//		pushConst();
//	}
//	_pushConstCalls.clear();
//}
//
//
//void Engine::camera_logic()
//{
//	using tools::Keys;
//	using tools::Mods;
//
//	tools::KeyUsageRegistry& keys = tools::KeyUsageRegistry::get_instance();
//	
//
//	for (const auto& [key, mod] : keys.keys_in_use())
//	{
//		std::function<bool()> func = [this, val = _window.FindKeyComb(key)]() -> bool
//			{
//				val->change_parameters(_deltaTime);
//				change_desc_set(Sets::Set1, Bindings::Binding1);
//				return true;
//			};
//
//		_window.SetFuncParamUpdaterKeys(key, std::move(func), mod);
//	}
//
//	_window.SetMouseChangeUpdater([this, mouseMove = _window.GetMouseMove()]() -> bool
//		{
//			mouseMove->change_parameters(_deltaTime, _window.GetMouseChangeXf(), _window.GetMouseChangeYf());
//			std::cout << "Change X: " << _window.GetMouseChangeXf() << " Change Y: " << _window.GetMouseChangeYf() << std::endl;
//			change_desc_set(Sets::Set1, Bindings::Binding1);
//			return true;
//		}
//	);
//
//
//	_window.SetEscapeButton(Keys::Esc);
//}
//
//
//Engine::~Engine()
//
//{
//	_vkLogicalDevice.waitIdle();
//
//	_vkGraphicsQueue.waitIdle();
//
//	_vertexBuffer.reset();
//	_indexBuffer.reset();
//	_scene.reset();
//
//}
//


//For now, we are not using the scene class,
// so we will just draw the mesh
//FIX IMMIDIATELY TO DRAW IN THE CORRECT ORDER

//if (_scene)
//{
//	for (const auto& position : _scene->get_triangles_pos())
//	{ 
//		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f));
//		vkUtil::ObjectData objectData(vkUtil::ShaderStage::VERTEX);
//		objectData.c_data._model = model;
//		send_as_push_const(objectData.c_data, cmdBuffer, vkUtil::ShaderStage::VERTEX, 0);
//	}
//}

//vkUtil::ObjectData objectData(vkUtil::ShaderStage::VERTEX);
//objectData.c_data._model = glm::mat4(1.0f);
//send_as_push_const(objectData.c_data, cmdBuffer, vkUtil::ShaderStage::VERTEX, 0); 



//
//void Engine::load_scene(std::unique_ptr<SceneT>& scene)
//{
//	if (scene)
//	{
//		_scene = std::move(scene);
//	}
//}