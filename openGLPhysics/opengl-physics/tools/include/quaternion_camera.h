#pragma once

#include "config.h"

#include "tools/include/camera_bundle.h"
#include "tools/include/base_camera.h"


namespace tools
{

	class QuaternionCamera : public BaseCamera
	{
	public:
		QuaternionCamera() = default;

		QuaternionCamera(QuaternionCamera&&) = default;
		QuaternionCamera& operator=(QuaternionCamera&&) = default;
		
		QuaternionCamera(const QuaternionCamera&) = default;
		QuaternionCamera& operator=(const QuaternionCamera&) = default;
		
		QuaternionCamera(const CameraBundlePerspective& bundle);
		QuaternionCamera(const CameraBundleOrthographic& bundle);

		void init(CameraBundlePerspective bundle);
		
		void init(CameraBundleOrthographic bundle);
		
		void set_speed(float speed);
		float get_speed() const;
		
		void set_position(const glm::vec3& position);
		
		bool is_moving();
	
		bool event_key(Direction dir, float deltaTime) override;
		bool event_key(float deltaTime, float xMove, float yMove) override;
		bool event_scroll(float yOffset) override;

		void on_scroll(double yOffset);

		~QuaternionCamera();


	private:
		glm::vec3 _prevPosition;
		glm::quat _orientation;
		glm::quat _prevOrientation;
		glm::vec3 _worldUp;
		glm::vec3 _front;
		glm::vec3 _up; 
		glm::vec3 _right; 
		glm::vec3 _target;

		float _fov;
		
		bool _ortho = false;
		float _zoomSpeed, _aspectRatio, _nearZ, _farZ;

		float _yawAngle = 0.0f;
		float _pitchAngle = 0.0f;

		float _speed;
		float _turnSpeed;
		float _aspect;


		void initialize(glm::vec3 worldUp, glm::vec3 startPYR, glm::vec3 position, glm::vec3 front, float speed, float turnSpeed);

		//void update_orientation_from_angles();

		void move_forward(double deltaTime, bool forwardOrBack);
		void move_up(double deltaTime, bool upOrDown);
		void move_right(double deltaTime, bool rightOrLeft);

		void move_and_turn_dir(Direction dir, double deltaTime);

		void pitch(float deltaTime, float xMove);
		void yaw(float deltaTime, float yMove);
		void roll(float deltaTime, float zMove);

		void update_view_matrix();
	};


}

