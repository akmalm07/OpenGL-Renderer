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

		_nearZ = bundle.nearZ;
		_farZ = bundle.farZ;
		_zoomSpeed = bundle.zoomSpeed;
		_aspectRatio = bundle.aspectRatio;
		_fov = bundle.fov;
		_ortho = false;

		initialize(bundle.worldUp, bundle.startPYR, bundle.position, bundle.front, bundle.speed, bundle.turnSpeed);
	}

	void QuaternionCamera::init(CameraBundleOrthographic bundle)
	{
		_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		_projection = glm::ortho(bundle.left, bundle.right, bundle.bottom, bundle.top, bundle.nearZ, bundle.farZ);

		_nearZ = bundle.nearZ;
		_farZ = bundle.farZ;
		_aspect = (bundle.right - bundle.left) / (bundle.top - bundle.bottom);
		_ortho = true;

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
		_pitchAngle = startPYR.x;
		_yawAngle = startPYR.y;

		update_orientation_from_angles();
		update_view_matrix();
	}

	void QuaternionCamera::update_orientation_from_angles()
	{
		glm::quat yawQuat = glm::angleAxis(glm::radians(_yawAngle), glm::vec3(0, 1, 0));
		glm::quat pitchQuat = glm::angleAxis(glm::radians(_pitchAngle), glm::vec3(1, 0, 0));
		_orientation = glm::normalize(yawQuat * pitchQuat);
	}

	void QuaternionCamera::move_forward(double deltaTime, bool forwardOrBack)
	{
		_position += (forwardOrBack ? _front : -_front) * _speed * (float)deltaTime;
	}

	void QuaternionCamera::move_up(double deltaTime, bool upOrDown)
	{
		_position += (upOrDown ? _up : -_up) * _speed * (float)deltaTime;
	}

	void QuaternionCamera::move_right(double deltaTime, bool rightOrLeft)
	{
		_position += (rightOrLeft ? _right : -_right) * _speed * (float)deltaTime;
	}

	void QuaternionCamera::pitch(double deltaTime, float xMove)
	{
		_pitchAngle += _turnSpeed * xMove * (float)deltaTime;
		_pitchAngle = glm::clamp(_pitchAngle, -89.0f, 89.0f);
		update_orientation_from_angles();
	}

	void QuaternionCamera::yaw(double deltaTime, float yMove)
	{
		_yawAngle += _turnSpeed * yMove * (float)deltaTime;
		update_orientation_from_angles();
	}

	void QuaternionCamera::roll(double deltaTime, float zMove)
	{
		glm::quat rollQuat = glm::angleAxis(glm::radians(_turnSpeed * zMove * (float)deltaTime), glm::rotate(_orientation, glm::vec3(0, 0, -1)));
		_orientation = glm::normalize(rollQuat * _orientation);
	}

	void QuaternionCamera::update_view_matrix()
	{
		_front = glm::normalize(glm::rotate(_orientation, glm::vec3(0, 0, -1)));
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
		move_and_turn_dir(dir, deltaTime);
		update_view_matrix();
		return true;
	}

	bool QuaternionCamera::event_key(double deltaTime, float xMove, float yMove)
	{
		if (xMove != 0) yaw(deltaTime, xMove);
		if (yMove != 0) pitch(deltaTime, yMove);
		update_view_matrix();
		return true;
	}

	bool QuaternionCamera::event_scroll(float yOffset)
	{
		on_scroll(yOffset);
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
		if (BOOL(dir & Direction::Forward)) move_forward(deltaTime, true);
		else if (BOOL(dir & Direction::Backward)) move_forward(deltaTime, false);

		if (BOOL(dir & Direction::Left)) move_right(deltaTime, false);
		else if (BOOL(dir & Direction::Right)) move_right(deltaTime, true);

		if (BOOL(dir & Direction::Up)) move_up(deltaTime, true);
		else if (BOOL(dir & Direction::Down)) move_up(deltaTime, false);
	}

	bool QuaternionCamera::is_moving()
	{
		bool positionChanged = glm::length(_position - _prevPosition) > 0.0001f;
		bool orientationChanged = glm::dot(_orientation, _prevOrientation) < 0.9999f;

		_prevPosition = _position;
		_prevOrientation = _orientation;
		return positionChanged || orientationChanged;
	}

	void QuaternionCamera::on_scroll(double yOffset)
	{
		if (_ortho)
		{
			float zoomFactor = 1.0f + (yOffset * _zoomSpeed);
			_projection = glm::ortho(-_aspect * zoomFactor, _aspect * zoomFactor, -zoomFactor, zoomFactor, _nearZ, _farZ);
		}
		else
		{
			_fov -= yOffset * _zoomSpeed;
			_fov = glm::clamp(_fov, 1.0f, 90.0f);
			_projection = glm::perspective(glm::radians(_fov), _aspectRatio, _nearZ, _farZ);
		}
	}

	QuaternionCamera::~QuaternionCamera() = default;

}