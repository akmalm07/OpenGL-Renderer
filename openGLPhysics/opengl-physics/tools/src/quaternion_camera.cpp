#include "headers.h"
#include "tools/include/quaternion_camera.h"

#include <glm/gtx/transform.hpp> 
#include <glm/gtx/quaternion.hpp>


namespace tools
{



	QuaternionCamera::QuaternionCamera(const CameraBundlePerspective& bundle)
	{
		init(bundle);
	}


	QuaternionCamera::QuaternionCamera(const CameraBundleOrthographic& bundle)
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


	void QuaternionCamera::pitch(float deltaTime, float xMove)
	{
		glm::vec3 right = glm::rotate(_orientation, glm::vec3(1, 0, 0));
		glm::quat q = glm::angleAxis(glm::radians(_turnSpeed * xMove * (float)deltaTime), right);
		_orientation = glm::normalize(q * _orientation);
	}

	void QuaternionCamera::yaw(float deltaTime, float yMove)
	{
		glm::vec3 up = glm::rotate(_orientation, glm::vec3(0, 1, 0));
		glm::quat q = glm::angleAxis(glm::radians(_turnSpeed * yMove * (float)deltaTime), up);
		_orientation = glm::normalize(q * _orientation);
	}

	void QuaternionCamera::roll(float deltaTime, float zMove)
	{
		glm::vec3 forward = glm::rotate(_orientation, glm::vec3(0, 0, -1));
		glm::quat q = glm::angleAxis(glm::radians(_turnSpeed * zMove * (float)deltaTime), forward);
		_orientation = glm::normalize(q * _orientation);
	}


	void QuaternionCamera::update_view_matrix()
	{
		_front = glm::rotate(_orientation, glm::vec3(0, 0, -1));
		_right = glm::rotate(_orientation, glm::vec3(1, 0, 0));
		_up = glm::rotate(_orientation, glm::vec3(0, 1, 0));

		_target = _position + _front;

		_view = glm::lookAt(_position, _target, _up);
	}


	bool QuaternionCamera::event_key(Direction dir, float deltaTime)
	{
		move_and_turn_dir(dir, deltaTime);
		update_view_matrix();
		return true;
	}


	bool QuaternionCamera::event_key(float deltaTime, float xMove, float yMove)
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
		return true;
	}

	bool QuaternionCamera::event_scroll(float yOffset)
	{
		on_scroll(yOffset);
		return true	;
	}

	void QuaternionCamera::on_scroll(double yOffset)
	{
		if (_ortho)
		{
			float zoomFactor = 1.0f + (float)yOffset * _zoomSpeed;
			_projection = glm::ortho(-_aspectRatio * zoomFactor, _aspectRatio * zoomFactor, -zoomFactor, zoomFactor, _nearZ, _farZ);
		}
		else
		{
			_fov = glm::clamp(_fov - (float)yOffset * _zoomSpeed, 1.0f, 45.0f);
			_projection = glm::perspective(glm::radians(_fov), _aspectRatio, _nearZ, _farZ);
		}
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
		bool move = false;
		if (BOOL(dir & Direction::Forward))
		{
			move_forward(deltaTime, true);
			move = true;
		}
		else if (BOOL(dir & Direction::Backward))
		{
			move_forward(deltaTime, false);
			move = true;
		}


		if (BOOL(dir & Direction::Left))
		{
			move_right(deltaTime, false);
			move = true;
		}
		else if (BOOL(dir & Direction::Right))
		{
			move_right(deltaTime, true);
			move = true;
		}


		if (BOOL(dir & Direction::Up))
		{
			move_up(deltaTime, true);
			move = true;
		}
		else if (BOOL(dir & Direction::Down))
		{
			move_up(deltaTime, false);
			move = true;
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