#pragma once

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class Camera
{
public:
	Camera();
	~Camera();


	void processKeyBoardInputs(GLFWwindow* window, float deltaTime);
	void processMouseMovement(GLFWwindow* window, double x, double y);

	glm::mat4 getViewMatrix();
	glm::vec3 getPos() { return camPos; };


private:
	glm::vec3 camPos, camFront, camUp;
	float camSpeed;
	float pitch, yaw;

	bool firstMouse;

	float lastX;
	float lastY;

};

