#include "headers.h"
#include "tools/include/quaternion_camera.h"

#include <glm/gtx/transform.hpp> 
#include <glm/gtx/quaternion.hpp>


namespace tools
{

	QuaternionCamera::QuaternionCamera(CameraBundlePerspective bundle)
	{
		init(bundle);
	}


	QuaternionCamera::QuaternionCamera(CameraBundleOrthographic bundle) 
	{
		init(bundle);
	}

	void QuaternionCamera::init(CameraBundlePerspective bundle)
	{
		_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		_projection = glm::perspective(glm::radians(bundle.fov), bundle.aspectRatio, bundle.nearZ, bundle.farZ);
		initialize(bundle.worldUp, bundle.startPYR, bundle.position, bundle.front, bundle.speed, bundle.turnSpeed);
	}

	void QuaternionCamera::init(CameraBundleOrthographic bundle)
	{
		_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		_projection = glm::ortho(bundle.left, bundle.right, bundle.bottom, bundle.top, bundle.nearZ, bundle.farZ);
		initialize(bundle.worldUp, bundle.startPYR, bundle.position, bundle.front, bundle.speed, bundle.turnSpeed);
	}


	void QuaternionCamera::initialize(glm::vec3 worldUp, glm::vec3 startPYR, glm::vec3 position, glm::vec3 front, float speed, float turnSpeed)
	{
		_worldUp = glm::normalize(worldUp);
		_position = position;
		_speed = speed;
		_turnSpeed = turnSpeed;
		_front = front;
		_prevPosition = _position;
		_prevOrientation = _orientation;


		glm::quat pitchQuat = glm::angleAxis(glm::radians(startPYR.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat yawQuat = glm::angleAxis(glm::radians(startPYR.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat rollQuat = glm::angleAxis(glm::radians(startPYR.z), glm::vec3(0.0f, 0.0f, 1.0f));
		_orientation = glm::normalize(yawQuat * pitchQuat * rollQuat);


		update_view_matrix();
	}


	void QuaternionCamera::move_forward(double deltaTime, bool forwardOrBack)
	{
		if (forwardOrBack) 
		{
			_position += _front * _speed * (float)deltaTime;
		}
		else
		{
			_position -= _front * _speed * (float)deltaTime;
		}
	}


	void QuaternionCamera::move_up(double deltaTime, bool upOrDown)
	{
		if (upOrDown) 
		{
			_position += _up * _speed * (float)deltaTime;
		}
		else 
		{
			_position -= _up * _speed * (float)deltaTime;
		}
	}


	void QuaternionCamera::move_right(double deltaTime, bool rightOrLeft) 
	{
		if (rightOrLeft) 
		{
			_position += _right * _speed * (float)deltaTime;
		}
		else 
		{
			_position -= _right * _speed * (float)deltaTime;
		}
	}


	void QuaternionCamera::pitch(double deltaTime, float xMove)
	{
		glm::quat pitchQuat = glm::angleAxis(glm::radians(_turnSpeed * xMove * (float)deltaTime), glm::vec3(1.0f, 0.0f, 0.0f));
		_orientation = glm::normalize(pitchQuat * _orientation);
	}


	void QuaternionCamera::yaw(double deltaTime, float yMove)
	{
		glm::quat yawQuat = glm::angleAxis(glm::radians(_turnSpeed * yMove * (float)deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
		_orientation = glm::normalize(yawQuat * _orientation);
	}


	void QuaternionCamera::roll(double deltaTime, float zMove) 
	{
		glm::quat rollQuat = glm::angleAxis(glm::radians(_turnSpeed * zMove * (float)deltaTime), glm::vec3(0.0f, 0.0f, 1.0f));
		_orientation = glm::normalize(rollQuat * _orientation);
	}


	void QuaternionCamera::update_view_matrix() 
	{
		_front = glm::normalize(glm::vec3(glm::rotate(_orientation, glm::vec3(0.0f, 0.0f, -1.0f))));
		_right = glm::normalize(glm::cross(_front, _worldUp));
		_up = glm::normalize(glm::cross(_right, _front));

		_target = _position + _front;

		_view = glm::lookAt(_position, _target, _up);
	}


	void QuaternionCamera::update(Direction dir, double deltaTime) 
	{
		move_and_turn_dir(dir, deltaTime);
	}


	bool QuaternionCamera::event_key(Direction dir, double deltaTime)
	{
		PRINT_VEC3("Camera position: ", _position);

		update(dir, deltaTime);
		
		update_view_matrix(); // This is being called more then once, but only captured once, waste of CPU resources
		return true;
	}


	bool QuaternionCamera::event_key(double deltaTime, float xMove, float yMove) 
	{
		if (xMove != 0) 
		{
			yaw(deltaTime, xMove);
		}
		if (yMove != 0) 
		{
			pitch(deltaTime, yMove);
		}
		update_view_matrix();

		_view = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 5.0f),  // camera position
			glm::vec3(0.0f, 0.0f, 0.0f),  // look at origin
			glm::vec3(0.0f, 1.0f, 0.0f)   // up
		);

		return true;
	}


	void QuaternionCamera::set_speed(float speed) 
	{
		_speed = speed;
	}


	float QuaternionCamera::get_speed() const
	{
		return _speed;
	}


	void QuaternionCamera::set_position(const glm::vec3& position)
	{
		_position = position;
		update_view_matrix();
	}


	void QuaternionCamera::move_and_turn_dir(Direction dir, double deltaTime) 
	{
		if (BOOL(dir & Direction::Forward))
		{
			move_forward(deltaTime, true);

		}
		else if (BOOL(dir & Direction::Backward)) 
		{
			move_forward(deltaTime, false);

		}


		if (BOOL(dir & Direction::Left))
		{
			move_right(deltaTime, false);

		}
		else if (BOOL(dir & Direction::Right))
		{
			move_right(deltaTime, true);

		}


		if (BOOL(dir & Direction::Up))
		{
			move_up(deltaTime, true);

		}
		else if (BOOL(dir & Direction::Down))
		{
			move_up(deltaTime, false);

		}
	}


	bool QuaternionCamera::is_moving() 
	{
		bool positionChanged = glm::length(_position - _prevPosition) > 0.0001f;
		bool orientationChanged = glm::dot(_orientation, _prevOrientation) < 0.9999f;


		_prevPosition = _position;
		_prevOrientation = _orientation;


		return positionChanged || orientationChanged;
	}


	QuaternionCamera::~QuaternionCamera() = default;

}