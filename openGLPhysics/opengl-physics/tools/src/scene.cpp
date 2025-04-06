#include "headers.h"

#include "tools\include\scene.h"

//SceneT::SceneT()
//{
//	initalize();
//}
//
//
//SceneT::SceneT(SceneT&& other) noexcept = default;
//
//SceneT& SceneT::operator=(SceneT&& other) noexcept
//{
//	if (this != &other)
//	{
//		triangles = std::move(other.triangles);
//
//		other.triangles.clear();
//	}
//	return *this;
//}
//
//
//std::vector <MeshT> SceneT::get_triangles()
//{
//	return triangles;
//}
//
//std::vector <UserInput::pos> SceneT::get_triangles_pos()
//{
//	std::vector<UserInput::pos> positions;
//	for (size_t i = 0; const auto& mesh : triangles)
//	{
//		for (const auto& vert : mesh.get_vertices())
//		{
//			positions.push_back(vert._position);
//		}
//	}
//
//	return positions; 
//}
//
//void SceneT::update_sets(vk::CommandBuffer& cmdBuff) const
//{
//	//IF I HAVE SETS I WILL UPDATE THEM
//}
//
//SceneT::~SceneT() = default;
//
//
//
//void SceneT::initalize()
//{
//
//	triangles.push_back({ 
//		{ {0.5f, 1.0f}, { 1.0f, 0.0f, 0.5f }},
//		{ {-0.5f, 1.0f}, { 1.0f, 0.0f, 0.5f }},
//		{ {1.0f, -1.0f}, { 1.0f, 0.0f, 0.5f }}
//		});
//}