#include "headers.h"

#include "tools\include\window_input.h"

#include "tools\include\keys.h"

#include "tools\include\window.h"
#include "tools\include\timer.h"


namespace tools {

	uint32_t Window::g_numOfWindows = 0;

	bool Window::_calledBufferSize = true;

	Window::Window()
	{
		_name = "Untitled Window";

		_mainWindow = nullptr;

		_width = 800.0f;
		_height = 800.0f;

		_aspectRatio = 1.0f;

		_mouseCurrentX = _width / 2;
		_mouseCurrentY = _height / 2;
	}

	Window::Window(float windowWidth, float windowHeight, const std::string& name)
	{

		_name = name;

		_mainWindow = nullptr;

		_bufferWidth = 0;
		_bufferHeight = 0;

		_width = windowWidth;
		_height = windowHeight;
	
		_mouseCurrentX = _width / 2;
		_mouseCurrentY = _height / 2;
	}

	Window::Window(Window&& other) noexcept
	{
		_AABButtons = std::move(other._AABButtons); 
		_keyCombs = std::move(other._keyCombs);
		_mainWindow = other._mainWindow; 
		_width = other._width;
		_height = other._height;
		_bufferWidth = other._bufferWidth;
		_bufferHeight = other._bufferHeight;
		_aspectRatio = other._aspectRatio;
		_leftOrtho = other._leftOrtho;
		_rightOrtho = other._rightOrtho;
		_topOrtho = other._topOrtho;
		_bottomOrtho = other._bottomOrtho;
		_timer = std::move(other._timer); 
		_deltaTime = other._deltaTime; 
		_name = std::move(other._name);
		_updated = other._updated; 

		_mouseMove = other._mouseMove;  

		other._mouseMove = nullptr;
		other._mainWindow = nullptr; 
		other._width = 0.0f; 
		other._height = 0.0f;
		other._bufferWidth = 0; 
		other._bufferHeight = 0; 
		other._aspectRatio = 1.0f; 
		other._leftOrtho = std::nullopt; 

		other._rightOrtho = std::nullopt;
		other._topOrtho = std::nullopt;
		other._bottomOrtho = std::nullopt;

	}

	Window& Window::operator=(Window&& other) noexcept
	{
		if (this != nullptr)
		{
			_AABButtons = std::move(other._AABButtons);
			_keyCombs = std::move(other._keyCombs);
			_mainWindow = other._mainWindow;
			_width = other._width;
			_height = other._height;
			_bufferWidth = other._bufferWidth;
			_bufferHeight = other._bufferHeight;
			_aspectRatio = other._aspectRatio;
			_leftOrtho = other._leftOrtho;
			_rightOrtho = other._rightOrtho;
			_topOrtho = other._topOrtho;
			_bottomOrtho = other._bottomOrtho;
			_timer = std::move(other._timer);
			_deltaTime = other._deltaTime;
			_name = std::move(other._name);
			_updated = other._updated;

			_mouseMove = other._mouseMove;

			other._mouseMove = nullptr;
			other._mainWindow = nullptr;
			other._width = 0.0f;
			other._height = 0.0f;
			other._bufferWidth = 0;
			other._bufferHeight = 0;
			other._aspectRatio = 1.0f;
			other._leftOrtho = std::nullopt;

			other._rightOrtho = std::nullopt;
			other._topOrtho = std::nullopt;
			other._bottomOrtho = std::nullopt;
		}
		return *this;
	}

	void Window::set_ortho()
	{
		_isOrtho = true;

		_aspectRatio = _width / _height;


		if (_aspectRatio >= 1.0f)
		{
			_leftOrtho = -1.0f * _aspectRatio;
			_rightOrtho = 1.0f * _aspectRatio;
			_topOrtho = 1.0f;
			_bottomOrtho = -1.0f;
		}
		else
		{
			_leftOrtho = -1.0f;
			_rightOrtho = 1.0f;
			_topOrtho = 1.0f / _aspectRatio;
			_bottomOrtho = -1.0f / _aspectRatio;
		}
	
	}


	void Window::set_escape_button(Keys key, std::optional<Mods> mod)
	{
		AddKeyComb(
			false,
			{ key, Action::Press, mod.value_or(Mods::None) },
			[this]() -> bool
			{
				glfwSetWindowShouldClose(_mainWindow, GLFW_TRUE);
				return true;
			}
		);
	}

	bool Window::is_updated()
	{
		if (_updated)
		{
			_updated = false;
			return true;
		}
		return false;
	}

	void Window::reset_delta_time()
	{
		_deltaTime = _timer.get_delta_time(false);
	}

	double Window::get_delta_time() const
	{
		return _deltaTime;
	}

	const double& Window::get_delta_time_ref()
	{
		return _deltaTime;
	}

	bool Window::create_window(bool disableCursor, bool isOrtho)
	{
		std::cout << "Window count: " << g_numOfWindows << "\n";

		if (g_numOfWindows == 0)
		{
			if (glfwInit() == GLFW_FALSE)
			{
				std::cerr << "Error initializing GLFW! \n";
				return false;
			}

			glfwSetErrorCallback([](int error, const char* description) {
				fprintf(stderr, "GLFW Error: %s\n", description);
				});

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		_mainWindow = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
		if (!_mainWindow)
		{
			throw std::runtime_error("GLFW window creation failed!");
		}

		std::cout << "Successfully created GLFW window \"" << _name << "\", "
			<< "width: " << _width << ", height: " << _height << "\n";

		if (g_numOfWindows == 0)
		{
			glfwMakeContextCurrent(_mainWindow);
			glewExperimental = GL_TRUE;

			if (glewInit() != GLEW_OK)
			{
				std::cerr << "Error initializing GLEW! \n";
				return false;
			}
			
			glEnable(GL_CULL_FACE); //DEBUG
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		g_numOfWindows++;

		glfwGetFramebufferSize(_mainWindow, &_bufferWidth, &_bufferHeight);

		int theWidth = static_cast<int>(_width);
		int theHeight = static_cast<int>(_height);
		glfwGetWindowSize(_mainWindow, &theWidth, &theHeight);

		glfwSetCursorPos(_mainWindow, theWidth / 2, theHeight / 2);

		if (disableCursor)
		{
			glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		CreateCallbacks();

		glfwSetWindowUserPointer(_mainWindow, this);

		if (isOrtho)
		{
			set_ortho();
		}

		return true;
	}

	bool Window::create_window(float windowWidth, float windowHeight, const std::string& name, bool disableCursor, bool isOrtho)
	{
		std::cout << "Window count: " << g_numOfWindows << "\n";

		if (g_numOfWindows == 0)
		{
			if (glfwInit() == GLFW_FALSE)
			{
				std::cerr << "Error initializing GLFW! \n";
				return false;
			}

			glfwSetErrorCallback([](int error, const char* description) {
				fprintf(stderr, "GLFW Error: %s\n", description);
				});

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		_mainWindow = glfwCreateWindow(windowWidth, windowHeight, name.c_str(), nullptr, nullptr);
		if (!_mainWindow)
		{
			throw std::runtime_error("GLFW window creation failed!");
		}

		std::cout << "Successfully created GLFW window \"" << name << "\", "
			<< "width: " << windowWidth << ", height: " << windowHeight << "\n";

		if (g_numOfWindows == 0)
		{
			glfwMakeContextCurrent(_mainWindow);
			glewExperimental = GL_TRUE;

			if (glewInit() != GLEW_OK)
			{
				std::cerr << "Error initializing GLEW! \n";
				return false;
			}

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		g_numOfWindows++;

		glfwGetFramebufferSize(_mainWindow, &_bufferWidth, &_bufferHeight);

		int theWidth = static_cast<int>(windowWidth);
		int theHeight = static_cast<int>(windowHeight);
		glfwGetWindowSize(_mainWindow, &theWidth, &theHeight);

		glfwSetCursorPos(_mainWindow, theWidth / 2, theHeight / 2);

		if (disableCursor)
		{
			glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		CreateCallbacks();

		glfwSetWindowUserPointer(_mainWindow, this);

		if (isOrtho)
		{
			set_ortho();
		}

		_width = windowWidth; 
		_height = windowHeight;
		_aspectRatio = _width / _height; 
		_name = name;

		return true;
	}

	void Window::set_disable_cursor(bool disableCursor)
	{
		if (disableCursor)
		{
			glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	float Window::get_aspect_ratio() const
	{
		return _aspectRatio;
	}


	void Window::set_should_close(bool trueOrFalse)
	{
		return glfwSetWindowShouldClose(_mainWindow, (trueOrFalse ? GLFW_TRUE : GLFW_FALSE));
	}

	void Window::swap_buffers() const
	{
		glfwSwapBuffers(_mainWindow);
	}

	float Window::get_left_ortho() const
	{ 
		return (_leftOrtho.has_value() ? _leftOrtho.value() : throw std::runtime_error("_leftOrtho has not VALUE!")); 
	}

	float Window::get_bottom_ortho() const
	{ 
		return (_bottomOrtho.has_value() ? _bottomOrtho.value() : throw std::runtime_error("_bottomOrtho has not VALUE!")); 
	}

	float Window::get_top_ortho() const 
	{ 
		return (_topOrtho.has_value() ? _topOrtho.value() : throw std::runtime_error("_topOrtho has not VALUE!")); 
	}

	float Window::get_right_ortho() const
	{ 
		return (_rightOrtho.has_value() ? _rightOrtho.value() : throw std::runtime_error("_rightOrtho has not VALUE!")); 
	}

	bool Window::is_key_active(Keys key, Action action) const
	{
		if (glfwGetKey(_mainWindow, INT(key)) == INT(action))
		{
			return true;
		}
		return false;
	}

	std::string Window::get_name() const
	{
		return _name;
	}

	void Window::set_name(const char* name)
	{
		glfwSetWindowTitle(_mainWindow, name);
		_name = std::string(name);
	}

	bool Window::get_should_close() const
	{
		return glfwWindowShouldClose(_mainWindow);
	}

	void Window::set_cursor_locked()
	{
		glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}



	int Window::get_buffer_width()
	{ 
		glfwGetFramebufferSize(_mainWindow, &_bufferWidth, &_bufferHeight);

		return _bufferWidth; 
	}

	bool Window::is_ortho() const
	{
		return _isOrtho;
	}

	int Window::get_buffer_height()
	{ 
		glfwGetFramebufferSize(_mainWindow, &_bufferWidth, &_bufferHeight); 

		return _bufferHeight; 
	}

	const int* Window::get_buffer_width_p()
	{
		glfwGetFramebufferSize(_mainWindow, &_bufferWidth, &_bufferHeight);

		return &_bufferWidth;
	}

	const int* Window::get_buffer_height_p()
	{
		glfwGetFramebufferSize(_mainWindow, &_bufferWidth, &_bufferHeight);

		return &_bufferHeight;
	}

	GLFWwindow* Window::get_window() const
	{
		return _mainWindow;
	}

	bool Window::set_window(GLFWwindow* window, bool isOrtho)
	{ 
		if (!window)
		{
			fprintf(stderr, "\nThe Window provided was nullptr!\n");
			return false;
		}
		_mainWindow = std::move(window); 


		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); 


		glfwGetFramebufferSize(_mainWindow, &_bufferWidth, &_bufferHeight); 

		int width;
		int height;

		glfwGetWindowSize(_mainWindow, &width, &height); 

		glfwSetCursorPos(_mainWindow, width / 2, height / 2); 

		_width = (float)width;
		_height = (float)height;

		glfwMakeContextCurrent(_mainWindow); 
		  
		CreateCallbacks(); 

		//glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetWindowUserPointer(_mainWindow, this); 


		_aspectRatio = _width / _height;

		if (isOrtho)
		{
			if (_aspectRatio >= 1.0f)
			{
				_leftOrtho = -1.0f * _aspectRatio;
				_rightOrtho = 1.0f * _aspectRatio;
				_topOrtho = 1.0f;
				_bottomOrtho = -1.0f;
			}
			else
			{
				_leftOrtho = -1.0f;
				_rightOrtho = 1.0f;
				_topOrtho = 1.0f / _aspectRatio;
				_bottomOrtho = -1.0f / _aspectRatio;
			}
		}


		return true;

	}

	float Window::get_width() const
	{
		return _width;
	}

	float Window::get_height() const
	{ 
		return _height; 
	}

	void Window::set_ortho(float left, float right, float top, float bottom)
	{
		_leftOrtho = left;
		_rightOrtho = right;
		_topOrtho = top;
		_bottomOrtho = bottom;
	}


	void Window::make_window_context_current()
	{
		glfwMakeContextCurrent(_mainWindow);
	}


	void Window::clear_window()
	{

		if (!_mainWindow)
		{
			_mainWindow = nullptr;
			glfwDestroyWindow(_mainWindow);
		}
		if (g_numOfWindows > 0)
		{
			g_numOfWindows--;
		}

		//GraphicsHandlerAPI::DeleteWindow(_mainWindow);
	}

	void Window::poll_events() const
	{
		glfwPollEvents();
	}

	void Window::wait_events() const
	{
		glfwWaitEvents();
	}

	Window::~Window()
	{
		clear_window();
	}


	//Private Functions
	void Window::CreateCallbacks()
	{
		//glfwSetWindowUserPointer(_mainWindow, this); 
		glfwSetKeyCallback(_mainWindow, m_HandleKeys);
		glfwSetCursorPosCallback(_mainWindow, m_HandleMouseCursor);
		glfwSetMouseButtonCallback(_mainWindow, m_HandleMouseButtons);


	}


	void Window::m_HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
	{
		Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

		instance->HandleKeys(key, code, action, mode);
	}


	void Window::m_HandleMouseCursor(GLFWwindow* window, double posX, double posY)
	{
		Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

		instance->HandleMouseCursor(posX, posY);
	}


	void Window::m_HandleMouseButtons(GLFWwindow* window, int button, int action, int mods)
	{
		Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

		instance->HandleMouseButtons(button, action, mods);
	}


	void Window::HandleKeys(int key, int code, int action, int mode)
	{

		switch (action)
		{
		case GLFW_PRESS:
		{

			auto& act = _keyCombs[SIZET(Action::Press)];

			for (const auto& [ky, val] : act)
			{
				if (val->is_pressed(INT(key), INT(mode)))
				{
					_updated = true;
					_keys[key] = true;

					val->execute();
				}
			}
			
		}
		break;
		case GLFW_RELEASE:
		{
			auto& act = _keyCombs[SIZET(Action::Release)];

			if (_keyCombs.empty())
			{
				return;
			}
			
			_keys[key] = false;
			
			for (const auto& [ky, val] : act)
			{
				if (val->is_pressed(INT(key), INT(mode)))
				{
					val->execute();
				}
			}
			
		}
		break;
		case GLFW_REPEAT:
		{
			auto& act = _keyCombs[SIZET(Action::Repeat)];

			for (const auto& [ky, val] : act)
			{
				if (val->is_pressed(INT(key), INT(mode)))
				{
					_keys[key] = true;

					val->execute();
				}
			}
		
		}
		break;
		}//switch statement


		//Polykeys
		if (_keyCombsPoly.empty())
		{
			return;
		}

		for (const auto& [ky, val] : _keyCombsPoly)
		{
			if (val->is_pressed(_mainWindow, mode))
			{
				val->execute();
			}
		}
		
	}

	


	void Window::HandleMouseCursor(double posX, double posY)
	{
		_mouseChangeX = posX - _mouseCurrentX;
		_mouseChangeY = _mouseCurrentY - posY;

		_mouseCurrentX = posX;
		_mouseCurrentY = posY;
		
		if (_mouseMove)
		{
			_mouseMove->execute();
		}

	}

	void Window::HandleMouseButtons(int mouseButton, int action, int mods)
	{
		switch (action)
		{
		case GLFW_PRESS: {
			auto& act = _AABButtons[SIZET(Action::Press)];
			Mouse mouse = (mouseButton == GLFW_MOUSE_BUTTON_LEFT ? Mouse::Left : Mouse::Right);


			for (const auto& [key, val] : act)
			{
				if (val->is_clicked(_mouseCurrentX, _mouseCurrentY, Action::Press, mouse))
				{
					val->execute();
				}
			}

			break;
		}

		case GLFW_RELEASE:
		{

			auto& act = _AABButtons[SIZET(Action::Release)];
			Mouse mouse = (mouseButton == GLFW_MOUSE_BUTTON_LEFT ? Mouse::Left : Mouse::Right);


			for (const auto& [key, val] : act)
			{
				if (val->is_clicked(_mouseCurrentX, _mouseCurrentY, Action::Release, mouse))
				{
					val->execute();
				}
			}
			//_mouseButtons[SIZET(mouse)]->setPressed(false); 
			break;
		}

		case GLFW_REPEAT:
		{

			auto& act = _AABButtons[SIZET(Action::Repeat)];
			Mouse mouse = (mouseButton == GLFW_MOUSE_BUTTON_LEFT ? Mouse::Left : Mouse::Right);


			for (const auto& [key, val] : act)
			{
				if (val->is_clicked(_mouseCurrentX, _mouseCurrentY, Action::Repeat, mouse))
				{
					val->execute();
				}
			}
			//_mouseButtons[SIZET(mouse)]->setPressed(false); 
			break;
		}
		}//switch statement

	}

	// TO BE ADDED

	//Window::Async::Async() = default;

	//Window::Async::Async(const ThreadControlInfo& condition)
	//	: windowInputWait(condition.windowInputWait), changingParamsWait(condition.changingParamsWait), lock(condition.lock), state(false)
	//{}

	//Window::Async::Async(Async&& other) noexcept = default;

	//Window::Async& Window::Async::operator=(Async&& other) noexcept = default;

	//void Window::Async::notify_change(bool needsChange)
	//{
	//	if (needsChange)
	//	{
	//		state = true;
	//		changingParamsWait->notify_one();
	//		windowInputWait->wait_for_condition(*lock);

	//	}
	//}

	//void Window::Async::wait_for_window()
	//{
	//	windowInputWait->wait_for_condition(*lock);
	//}

	//void Window::Async::wait_for_change()
	//{

	//	changingParamsWait->wait_for_condition(*lock);
	//}

	//bool Window::Async::get_state() const
	//{
	//	return state;
	//}

	//void Window::Async::notify_window()
	//{
	//	state = false;
	//	
	//	windowInputWait->notify_one();
	//	changingParamsWait->wait_for_condition(*lock);
	//}
	//void Window::AllowWindowoContinueAndWait()
	//{
	//	_oneInputCurentlyActive.notify_window("I am from the camera logic");
	//}

	//void Window::WaitInitallyForSignal()
	//{
	//	_oneInputCurentlyActive.wait_for_change();
	//}
	//void Window::SetAsyncPollEvents(const ThreadControlInfo& cv)
	//{
	//	_oneInputCurentlyActive = cv;
	//}

} //Namespace tools