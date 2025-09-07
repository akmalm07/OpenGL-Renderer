#pragma once

#include "config.h"

#include "glUtil/include/shadow_map_bundle.h"

#include "glUtil/include/mesh.h"

#include "glInit/include/program.h"


namespace glUtil
{

	class ShadowMap 
	{
	public:
		ShadowMap();

		ShadowMap(const ShadowMapBundle& bundle, bool debug);
	
		void init(const ShadowMapBundle& bundle, bool debug);

		void bind();
		void unbind();

		unsigned int get_depth_map_texture() const;

		void set_shadow_map_loc(unsigned int location);
		void set_light_space_matrix_loc(unsigned int location);

		void link_light_space_matrix(const glm::mat4& lightSpaceMatrix);
		glm::mat4 get_light_space_matrix() const;

		void run_shadow_map_pass(std::vector<std::shared_ptr<glUtil::Mesh>>& meshes);

		void bind_shadow_tex();
		void unbind_shadow_tex();
		//void link_light_position(const glm::vec3& lightPos);

		void link_light_direction(const glm::vec3& lightDir);
		glm::vec3 get_light_direction() const;


		void clear();

		~ShadowMap();

	private:
		unsigned int _depthMapFBO = 0;
		unsigned int _depthMapTexture = 0;
		unsigned int _width = 0;
		unsigned int _height = 0;
		
		const unsigned int* _pOrigionalWidth = nullptr;
		const unsigned int* _pOrigionalHeight = nullptr;

		unsigned int _shadowMapLocation = 0;
		unsigned int _lightSpaceMatrixLocation = 0;

		TextureUnit _textureNumber = TextureUnit::TEX0;

		float _sceneSize = 1.0f;

		float _sceneDepthSize = 1.0f;

		bool _debugMode = false;

		glm::mat4 _lightSpaceMatrix = glm::mat4(1.0f);

		glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		std::vector<glm::vec3> _lightPos;

		glm::vec3 _lightDir = glm::vec3(0.0f, 0.0f, 0.0f);

		glInit::GLProgram _shadowPassProgram;
	};

}