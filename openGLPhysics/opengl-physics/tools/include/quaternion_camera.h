#pragma once

#include "config.h"

#include "tools/include/window.h"
#include "tools/include/camera_bundle.h"
#include "tools/include/base_camera.h"


namespace tools
{

	class QuaternionCamera : public BaseCamera
	{
	public:
		QuaternionCamera();
		QuaternionCamera(QuaternionCamera&& other) noexcept;
		QuaternionCamera& operator=(QuaternionCamera&& other) noexcept;
		
		QuaternionCamera(CameraBundlePerspective bundle);
		QuaternionCamera(CameraBundleOrthographic bundle);

		void init(CameraBundlePerspective bundle);
		
		void init(CameraBundleOrthographic bundle);

		void update(Direction dir, double deltaTime);
		
		void set_speed(float speed);
		float get_speed() const;
		
		void set_position(const glm::vec3& position);
		
		glm::mat4 get_view();
		glm::mat4 get_projection() const;
		glm::vec3 get_position() const;
		
		bool is_moving();
	
		bool event_key(Direction dir, double deltaTime) override;
		bool event_key(double deltaTime, float xMove, float yMove) override;

		~QuaternionCamera();


	private:
		glm::vec3 _prevPosition;
		glm::quat _orientation;
		glm::quat _prevOrientation;
		glm::vec3 _worldUp;
		glm::vec3 _front;
		glm::vec3 _up; 
		glm::vec3 _right; 

		float _speed;
		float _turnSpeed;
		float _aspect;


		void initialize(glm::vec3 worldUp, glm::vec3 startPYR, glm::vec3 position, glm::vec3 front, float speed, float turnSpeed);


		void move_forward(double deltaTime, bool forwardOrBack);
		void move_up(double deltaTime, bool upOrDown);
		void move_right(double deltaTime, bool rightOrLeft);

		void move_and_turn_dir(Direction dir, double deltaTime);

		void pitch(double deltaTime, float xMove);
		void yaw(double deltaTime, float yMove);
		void roll(double deltaTime, float zMove);

		void update_view_matrix();
	};


}

