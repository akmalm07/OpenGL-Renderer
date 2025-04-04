#include "headers.h"

#include "tools\include\camera.h"
#include "tools\include\keys.h"


namespace tools
{
	Camera::Camera() = default;


	Camera::Camera(Camera&& other) = default;

	Camera& Camera::operator=(Camera&& other) = default;

	Camera::Camera(CameraBundlePerspective bundle)
	{
		_projection = glm::perspective(glm::radians(bundle.fov), bundle.aspectRatio, bundle.nearZ, bundle.farZ);

		initalize(bundle.worldUp, bundle.startPYR, bundle.position, bundle.front, bundle.speed, bundle.turnSpeed);

	}


	Camera::Camera(CameraBundleOrthographic bundle)
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

		rotate_calc();

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


	void Camera::rotate_calc()
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

		_view = glm::lookAt(_position, _target, _up);

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

		rotate_calc();

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


	glm::mat4 Camera::get_view() 
	{
		return _view;
	}


	glm::mat4 Camera::get_projection() const
	{
		return _projection;
	}
	

	void Camera::set_commands_to_window(tools::Window& window)
	{

		std::array<KeyCombInputOne, 6> input = 
		{
		KeyCombInputOne(Keys::W, Action::Press),
		KeyCombInputOne(Keys::S, Action::Press),
		KeyCombInputOne(Keys::A, Action::Press),
		KeyCombInputOne(Keys::D, Action::Press),
		KeyCombInputOne(Keys::Q, Action::Press),
		KeyCombInputOne(Keys::E, Action::Press)
		};

		std::array<Direction, 6> dirs = 
		{
		Direction::Up,
		Direction::Down,
		Direction::Left,
		Direction::Right,
		Direction::Forward,
		Direction::Backward

		};


		std::array<std::function<bool(double)>, 6> funcs;

		for (size_t i = 0; i < funcs.size(); i++)
		{
			funcs[i] = [this, dir = dirs[i] ](double deltaTime) -> bool
			{
				return this->event_key(dir, deltaTime);
			};
		}

		std::function<bool(double, float, float)> mouseFuncs = [this] (double deltaTime, float xChange, float yChange) -> bool
		{
			return this->event_key(deltaTime, xChange, yChange);
		};

		window.AddMouseChange
		(
			{MouseChange::MoveX | MouseChange::MoveY, Mouse::None},
			mouseFuncs,
			0.0, 0.0f, 0.0f
		);

			
		for (size_t i = 0; i < funcs.size(); i++)
		{
			window.AddKeyComb
			(
				true,
				input[i],
				funcs[i],
				0.0
			);
		}

	}

	void Camera::move_and_turn_dir(Direction dir, double deltaTime)
	{
		static uint64_t count = 0;
		bool rotate = false;
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

		//if (BOOL(dir & Direction::TurnUp))
		//{
		//	pitch(deltaTime, true);
		//	rotate = true;
		//}
		//else if (BOOL(dir & Direction::TurnDown))
		//{
		//	pitch(deltaTime, false);
		//	rotate = true;
		//}

		//if (BOOL(dir & Direction::TurnLeft))
		//{
		//	yaw(deltaTime, false);
		//	rotate = true;
		//}
		//else if (BOOL(dir & Direction::TurnRight))
		//{
		//	yaw(deltaTime, true);
		//	rotate = true;
		//}
		//if (rotate)
		//{
		//	rotate_calc();
		//}
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

}//namespace vkUtil


//void Camera::debug_position()
//{
//	std::cout << "Camera Position: (" << _position.x << ", " << _position.y << ", " << _position.z << ")\n";
//}