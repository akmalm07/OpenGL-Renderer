#include "headers.h"

#include "tools\include\camera.h"
#include "tools\include\keys.h"


namespace tools
{

	Camera::Camera(CameraBundlePerspective bundle)
	{
		init(bundle);
	}


	Camera::Camera(CameraBundleOrthographic bundle)
	{
		init(bundle);
	}

	void Camera::init(CameraBundlePerspective bundle)
	{
		_projection = glm::perspective(glm::radians(bundle.fov), bundle.aspectRatio, bundle.nearZ, bundle.farZ);
		initalize(bundle.worldUp, bundle.startPYR, bundle.position, bundle.front, bundle.speed, bundle.turnSpeed);
	}

	void Camera::init(CameraBundleOrthographic bundle)
	{
		_projection = glm::ortho(bundle.left, bundle.right, bundle.bottom, bundle.top, bundle.nearZ, bundle.farZ);
		initalize(bundle.worldUp, bundle.startPYR, bundle.position, bundle.front, bundle.speed, bundle.turnSpeed);
	}

	void Camera::initalize(glm::vec3 worldUp, glm::vec3 startPYR, glm::vec3 position, glm::vec3 front, float speed, float turnSpeed)
	{
		_worldUp = glm::normalize(worldUp);
		_position = position;
		_rotation = startPYR;
		_speed = speed;
		_front = front;
		_turnSpeed = turnSpeed;
		_target = _position + _front;

		_prevPosition = _position;
		_prevRotation = _rotation;

		update_view_matrix();

		_view = glm::lookAt(_position, _target, _up);
	}

	void Camera::move_forward(double deltaTime, bool forwardOrBack)
	{
		if (forwardOrBack)
		{
			_position += _front * _speed * (float)deltaTime;
		}
		else
		{
			_position -= _front * _speed * (float)deltaTime;
		}
		_target = _position + _front;
	}

	void Camera::move_up(double deltaTime, bool upOrDown)
	{
		if (upOrDown)
		{

			_position += _up * _speed * (float)deltaTime;
		}
		else
		{
			_position -= _up * _speed * (float)deltaTime;
		}
		_target = _position + _front;
	}

	void Camera::move_right(double deltaTime, bool rightOrLeft)
	{
		if (rightOrLeft)
		{
			_position += _right * _speed * (float)deltaTime;
		}
		else
		{
			_position -= _right * _speed * (float)deltaTime;
		}
		_target = _position + _front;
	}


	void Camera::pitch(double deltaTime, bool forwardOrBack)
	{
		if (forwardOrBack)
		{
			_rotation.x += _turnSpeed * (float)deltaTime;
		}
		else
		{
			_rotation.x -= _turnSpeed * (float)deltaTime;
		}
	}

	void Camera::yaw(double deltaTime, bool upOrDown)
	{
		if (upOrDown)
		{
			_rotation.y += _turnSpeed * (float)deltaTime;
		}
		else
		{
			_rotation.y -= _turnSpeed * (float)deltaTime;
		}
	}

	void Camera::roll(double deltaTime, bool rightOrLeft)
	{
		if (rightOrLeft)
		{
			_rotation.z += _turnSpeed * (float)deltaTime;
		}
		else
		{
			_rotation.z -= _turnSpeed * (float)deltaTime;
		}
	}

	void Camera::pitch(double deltaTime, float xMove)
	{
		_rotation.x += _turnSpeed * xMove * (float)deltaTime;
		_rotation.x = std::clamp(_rotation.x, -89.0f, 89.0f);
	}

	void Camera::yaw(double deltaTime, float yMove)
	{
		_rotation.y += _turnSpeed * yMove * (float)deltaTime;
		_rotation.y = std::clamp(_rotation.y, -179.0f, 179.0f);
	}
	
	void Camera::roll(double deltaTime, float zMove)
	{
		_rotation.z += _turnSpeed * zMove * (float)deltaTime;
	}


	void Camera::update_view_matrix()
	{
		_front.x = cos(glm::radians(_rotation.y)) * cos(glm::radians(_rotation.x));
		_front.y = sin(glm::radians(_rotation.x));
		_front.z = sin(glm::radians(_rotation.y)) * cos(glm::radians(_rotation.x));
		_front = glm::normalize(_front);

		_right = glm::normalize(glm::cross(_front, _worldUp));
		_up = glm::normalize(glm::cross(_right, _front));

		_target = _position + _front;

		_view = glm::lookAt(_position, _target, _up);

	}

	void Camera::update(Direction dir, double deltaTime)
	{
		move_and_turn_dir(dir, deltaTime);

		update_view_matrix();

		//PRINT_MAT4("view: ", _view);

	}


	bool Camera::event_key(Direction dir, double deltaTime)
	{
		update(dir, deltaTime);
		return true;
	}

	bool Camera::event_key(double deltaTime, float xMove, float yMove)
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

		//PRINT_VEC3("Rotation: ", _rotation);
		return true;
	}


	void Camera::set_speed(float speed)
	{
		_speed = speed;
	}


	float Camera::get_speed() const
	{
		return _speed;
	}


	void Camera::set_position(const glm::vec3& position)
	{
		_position = position;
	}


	void Camera::move_and_turn_dir(Direction dir, double deltaTime)
	{
		bool move = false;

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

	bool Camera::is_moving()
	{
		bool positionChanged = glm::length(_position - _prevPosition) > 0.0001f; 
		bool rotationChanged = glm::dot(_rotation, _prevRotation) < 0.9999f; 

		_prevPosition = _position;
		_prevRotation = _rotation;

		return positionChanged || rotationChanged;
	}


	Camera::~Camera() = default;

}//namespace tools


//void Camera::debug_position()
//{
//	std::cout << "Camera Position: (" << _position.x << ", " << _position.y << ", " << _position.z << ")\n";
//}