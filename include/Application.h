#pragma once

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "Block.h"
#include "Renderer.h"
#include "Chunk.h"

#include <vector>

class Application
{
public:
	Application();
	~Application();

	void run();

	void init();
	void cleanup();

private:
	GLFWwindow* window;
	Renderer renderer;

	static void windowResize(GLFWwindow* window, int x, int y);
	


};