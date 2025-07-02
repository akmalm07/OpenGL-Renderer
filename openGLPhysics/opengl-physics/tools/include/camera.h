#pragma once

#include "config.h"

#include "tools\include\camera_bundle.h"
#include "tools\include\keys.h"
#include "tools\include\base_camera.h"



namespace tools
{
	class Camera : public BaseCamera
	{
	public:
		Camera() = default;
		
		Camera(Camera&&) = default;

		Camera& operator=(Camera&&) = default;

		Camera(CameraBundlePerspective bundle);

		Camera(CameraBundleOrthographic bundle);

		void init(CameraBundlePerspective bundle);

		void init(CameraBundleOrthographic bundle);

		void update_view_matrix();
		
		void update(Direction dir, float deltaTime);
		
		void set_speed(float speed);
		float get_speed() const;

		void set_position(const glm::vec3& position);

		void move_and_turn_dir(Direction dir, float deltaTime);

		bool is_moving();

		bool event_key(Direction dir, float deltaTime) override;
		bool event_key(float deltaTime, float xMove, float yMove) override;
		bool event_scroll(float yOffset) override;

		void on_scroll(double yOffset);

		~Camera();



	private:

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

		float _fov;

		bool _ortho = false;
		float _zoomSpeed, _aspectRatio, _nearZ, _farZ;

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

		//void debug_position();
	};

}
