#define GLM_ENABLE_EXPERIMENTAL // to use glm/gtx/*.h


#include <GL/glew.h>


#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <cmath>
#include <algorithm>
#include <thread>
#include <optional>
#include <variant>
#include <filesystem>


#define DEBUG(x) \
if (true) { \
	x \
} 
