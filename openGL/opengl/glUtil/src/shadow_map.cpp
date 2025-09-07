#include "headers.h"
#include "glUtil/include/shadow_map.h"

namespace glUtil
{
	ShadowMap::ShadowMap() = default;


	ShadowMap::ShadowMap(const ShadowMapBundle& bundle, bool debug)
	{
		init(bundle, debug);
	}

	void ShadowMap::init(const ShadowMapBundle& bundle, bool debug)
	{
		_width = bundle.shadowWidth;
		_height = bundle.shadowHeight;
		_pOrigionalWidth = bundle.pOriginalWidth;
		_pOrigionalHeight = bundle.pOriginalHeight;
		_lightPos = std::move(bundle.lightPos);
		_lightDir = bundle.lightDir;
		_lightSpaceMatrix = bundle.lightSpaceMatrix;
		_sceneSize = bundle.sceneSize;
		_sceneDepthSize = bundle.sceneDepthSize;
		_worldUp = bundle.worldUp;
		_textureNumber = bundle.textureNumber;

		_debugMode = debug;

		_shadowPassProgram.create_shaders_from_files(
			bundle.vertShadowShaderPath,
			bundle.fragShadowShaderPath
		);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		set_light_space_matrix_loc(_shadowPassProgram.add_uniform("uLightSpaceMatrix"));

		glGenFramebuffers(1, &_depthMapFBO);
		glGenTextures(1, &_depthMapTexture);
		glBindTexture(GL_TEXTURE_2D, _depthMapTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		if (_debugMode)
		{
			std::cout << "Creating Shadow Map FBO ID: " << _depthMapFBO << "\n";
			std::cout << "Creating Shadow Map Texture ID: " << _depthMapTexture << "\n";
			std::cout << "Shadow Map Width: " << _width << "\n";
			std::cout << "Shadow Map Height: " << _height << "\n";
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMapTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Error: Shadow Map Framebuffer is not complete!" << std::endl;
		}	

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0); 
	}

	void ShadowMap::bind() 
	{
		_shadowPassProgram.bind();

		glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
		glViewport(0, 0, _width, _height);
		glClear(GL_DEPTH_BUFFER_BIT);

	}

	void ShadowMap::set_shadow_map_loc(unsigned int location) 
	{
		_shadowMapLocation = location;
	}


	void ShadowMap::set_light_space_matrix_loc(unsigned int location)
	{
		_lightSpaceMatrixLocation = location;
	}


	glm::mat4 ShadowMap::get_light_space_matrix() const 
	{
		return _lightSpaceMatrix;
	}

	void ShadowMap::run_shadow_map_pass(std::vector<std::shared_ptr<glUtil::Mesh>>& meshes)
	{
		bind();

		glm::mat4 lightProjection, lightView;

		lightProjection = glm::ortho(-10.0f * _sceneSize, 10.0f * _sceneSize, -10.0f * _sceneSize, 10.0f * _sceneSize, 1.0f, 8.0f * _sceneDepthSize);

		glm::vec3 lightPos = glm::vec3(0.0f) - _lightDir * 10.0f; 

		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), _worldUp);

		_lightSpaceMatrix = lightProjection * lightView;

		PRINT_MAT4("light space mat", _lightSpaceMatrix);

		glUniformMatrix4fv(_lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(_lightSpaceMatrix));
		
		for (auto& mesh : meshes)
		{
			mesh->render();
		}
		
		unbind();
	}

	void ShadowMap::bind_shadow_tex()
	{
		glActiveTexture(GL_TEXTURE0 + UINT(_textureNumber));
		glBindTexture(GL_TEXTURE_2D, _depthMapTexture);
		glUniform1i(_shadowMapLocation, UINT(_textureNumber)); //Change
	}

	void ShadowMap::unbind_shadow_tex()
	{

		glActiveTexture(GL_TEXTURE0 + UINT(_textureNumber));
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void ShadowMap::link_light_direction(const glm::vec3& lightDir)
	{
		_lightDir = lightDir;

		glUniform3fv(_shadowMapLocation, 1, glm::value_ptr(_lightDir));
	}


	glm::vec3 ShadowMap::get_light_direction() const
	{
		return _lightDir;
	}

	void ShadowMap::link_light_space_matrix(const glm::mat4& lightSpaceMatrix)
	{
		_lightSpaceMatrix = lightSpaceMatrix;
		if (_lightSpaceMatrixLocation != 0)
		{
			glUniformMatrix4fv(_lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(_lightSpaceMatrix));
		}
	}



	void ShadowMap::unbind() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glViewport(0, 0, *_pOrigionalWidth, *_pOrigionalHeight);

		_shadowPassProgram.unbind();
	}


	unsigned int ShadowMap::get_depth_map_texture() const 
	{
		return _depthMapTexture;
	}


	void ShadowMap::clear()
	{
		if (_depthMapFBO != 0) 
		{
			glDeleteFramebuffers(1, &_depthMapFBO);
			_depthMapFBO = 0;
		}
		if (_depthMapTexture != 0)
		{
			glDeleteTextures(1, &_depthMapTexture);
			_depthMapTexture = 0;
		}
		_width = 0;
		_height = 0;
	}


	ShadowMap::~ShadowMap()
	{
		clear();
	}

}