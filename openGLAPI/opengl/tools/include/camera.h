#pragma once

#include "config.h"

#include "tools\include\camera_bundle.h"
#include "tools\include\keys.h"
#include "tools\include\window.h"



namespace tools
{
	class Camera
	{
	public:
		Camera();
		
		Camera(Camera&& other);

		Camera& operator=(Camera&& other);

		Camera(CameraBundlePerspective bundle);

		Camera(CameraBundleOrthographic bundle);

		void rotate_calc();
		
		void update(Direction dir, double deltaTime);
		
		void set_speed(float speed);
		float get_speed() const;

		void set_position(const glm::vec3& position);

		glm::mat4 get_view();

		glm::mat4 get_projection() const;

		glm::vec3 get_position() const;

		void set_commands_to_window(tools::Window& win);

		void move_and_turn_dir(Direction dir, double deltaTime);

		bool is_moving();

		~Camera();

	private:

		glm::mat4 _projection;
		glm::mat4 _view;

		glm::vec3 _position;
		glm::vec3 _prevPosition;
		glm::vec3 _rotation;
		glm::vec3 _prevRotation;
		glm::vec3 _scale;
		glm::vec3 _front;
		glm::vec3 _up;
		glm::vec3 _right;
		glm::vec3 _worldUp;
		glm::vec3 _direction;
		glm::vec3 _target;

		float _speed;
		float _turnSpeed;
		float _aspect;

	private:
		void initalize(glm::vec3 worldUp, glm::vec3 startPYR, glm::vec3 position, glm::vec3 front, float speed, float turnSpeed);

		void move_forward(double deltaTime, bool forwardOrBack);
		void move_up(double deltaTime, bool upOrDown);
		void move_right(double deltaTime, bool rightOrLeft);

		void pitch(double deltaTime, bool forwardOrBack);
		void yaw(double deltaTime, bool upOrDown);
		void roll(double deltaTime, bool rightOrLeft);

		void pitch(double deltaTime, float xMove);
		void yaw(double deltaTime, float yMove);
		void roll(double deltaTime, float zMove);

		bool event_key(Direction dir, double deltaTime);
		bool event_key(double deltaTime, float xMove, float yMove);

		//void debug_position();
	};

}

namespace vkType
{
	using Camera = tools::Camera;
}