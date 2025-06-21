#pragma once

#include "config.h"
#include "tools\include\keys.h"

#include "tools\include\window_input.h"
#include "tools\include\thread.h"
#include "tools\include\key_control.h"
#include "tools\include\aabb_button_control.h"
#include "tools\include\mouse_control.h"
#include "tools\include\timer.h"
#include "tools\include\input_manager.h"


namespace tools {


	class Window /*public KeyControl, public AABButtonControl, public MouseControl*/
	{
	public:
		Window();

		Window(float windowWidth, float windowHeight, const std::string& name);

		Window(Window&& other) noexcept;

		Window& operator=(Window&& other) noexcept;


		void set_ortho();


		bool create_window(bool disableCursor, bool isOrtho);
		
		
		bool create_window(float windowWidth, float windowHeight, const std::string& name, bool disableCursor, bool isOrtho);


		void set_disable_cursor(bool disableCursor);

		float get_aspect_ratio() const;

		void set_ortho(float left, float right, float top, float bottom);

		void make_window_context_current();

		void set_cursor_locked();

		int get_buffer_width();

		int get_buffer_height();

		const int* get_buffer_width_p();

		const int* get_buffer_height_p();

		bool is_ortho() const;

		GLFWwindow* get_window() const;

		bool set_window(GLFWwindow* window, bool isOrtho);

		float get_width() const;

		float get_height() const;

		float get_left_ortho() const;

		float get_bottom_ortho() const;

		float get_top_ortho() const;

		float get_right_ortho() const;

		bool is_key_active(Keys key, Action act) const;

		std::string get_name() const;

		void set_name(const char* name);

		bool get_should_close() const;

		void set_should_close(bool trueOrFalse);

		void swap_buffers() const;

		void clear_window();

		void poll_events() const;

		void wait_events() const;

		void set_escape_button(Keys key, std::optional<Mods> mod = std::nullopt);

		bool is_updated();

		void update(); // FIX: add functionality so that it can automate the updating process of the window, like updating the input manager, etc.

		void update_debug(); // FIX: add functionality so that it can automate the updating process FOR DEBUG of the window, like updating the input manager, etc.


		template<CallbackInputConcept InputStruct, typename... Args>
		void register_callback(const InputStruct& input, std::function<void(Args...)>& cb, std::function<void()>& updater, const std::string& name);

		template<CallbackInputConcept InputStruct>
		void register_callback(const InputStruct& input, std::function<void()>& cb, const std::string& name);

		template<CallbackInputConcept InputStruct>
		void emit(const InputStruct& input);

		template<CallbackInputConcept InputStruct>
		void emit(const InputStruct& input, const std::string& name);

		void reset_delta_time();

		double get_delta_time() const;
		
		const double& get_delta_time_ref();

		~Window();

	private:

		InputManager _inputManager;

		//Window Vars
		static bool _calledBufferSize;

		static uint32_t g_numOfWindows;

		bool _isOrtho = false;

		float _aspectRatio;

		GLFWwindow* _mainWindow = nullptr;

		float _width = 0.0f, _height = 0.0f;
		int _bufferWidth = 0, _bufferHeight = 0;

		std::optional<float> _leftOrtho, _rightOrtho, _topOrtho, _bottomOrtho;

		tools::Timer _timer; 

		double _deltaTime = 0.0; 

		std::string _name = "";

		bool _updated = false; // develop this to know if the window has been updated or not, used in the main loop to check for updates.

		std::array<bool, 1024> _keys{}; 

		double _mouseChangeX = 0.0;
		double _mouseChangeY = 0.0;

		double _mouseCurrentX = 0.0;
		double _mouseCurrentY = 0.0;

	private:
		void HandleKeys(int key, int code, int action, int mode);
		void HandleMouseCursor(double posX, double posY);
		void HandleMouseButtons(int mouseButton, int action, int mods);

		void CreateCallbacks();
		static void m_HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
		static void m_HandleMouseCursor(GLFWwindow* window, double posX, double posY);
		static void m_HandleMouseButtons(GLFWwindow* window, int button, int action, int mods);

	};

}

namespace vkType
{
	using Window = tools::Window;
}




namespace tools
{
	template<CallbackInputConcept InputStruct, typename... Args>
	inline void Window::register_callback(const InputStruct& input, std::function<void(Args...)>& cb, std::function<void()>& updater, const std::string& name)
	{
		_inputManager.register_callback<InputStruct, Args...>(input, std::move(cb), name, std::move(updater)); // ERR: Check
	}


	template<CallbackInputConcept InputStruct>
	inline void Window::register_callback(const InputStruct& input, std::function<void()>& cb, const std::string& name)
	{
		_inputManager.register_callback<InputStruct>(input, std::move(cb), name);
	}

	template<CallbackInputConcept InputStruct>
	inline void Window::emit(const InputStruct& input)
	{
		_inputManager.update_and_emit<InputStruct>(input);
	}
	template<CallbackInputConcept InputStruct>
	inline void Window::emit(const InputStruct& input, const std::string& name)
	{
		_inputManager.emit_and_update<InputStruct>(name);
	}
}



//------------------------------------------------


//PLANNING TO EXTEND
//void SetAsyncPollEvents(const ThreadControlInfo& cv);

//void AllowWindowoContinueAndWait();
//void WaitInitallyForSignal();
//template<class ... Args>
//void AddMouseClick(Mouse mouse, std::function<bool(Args...)> function);
//void DelMouseClick(Mouse mouse);
//MouseButton& FindMouseButtonName(Mouse mouse);
//Planning to add a function to find the mouse button soon...
//struct Async
//{
//public:
//	Async();
//	Async(const ThreadControlInfo& condition);
//	Async(Async&& other) noexcept;
//	Async& operator=(Async&& other) noexcept;

//	void notify_window();
//	void notify_change(bool needsChange);

//	void wait_for_window();
//	void wait_for_change();

//	bool get_state() const;

//	void wait_for_window();
//	void wait_for_change();

//private:
//	std::shared_ptr <ConditionalVariuble> windowInputWait;
//	std::shared_ptr <ConditionalVariuble> changingParamsWait;
//	std::shared_ptr <std::mutex> lock;
//	bool state;
//} _oneInputCurentlyActive;

//std::array<MouseButtonB, SIZET(Mouse::Count)> _mouseButtons; 

//void setMouseBeforeX(double posX);
//void setMouseBeforeY(double posY);

//void setMouseAfterX(double posX);
//void setMouseAfterY(double posY);
//WindowButton& FindWindowButtonName(const std::string& name);