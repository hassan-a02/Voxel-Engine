#include "Camera.h"

Camera::Camera()
{
	camPos = glm::vec3(0.0f, 80.0f, 0.0f);
	camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	pitch = 0.0f;
	yaw = -90.0f;

	camSpeed = 50.0f;

	firstMouse = true;

	lastX = 0.0f;
	lastY = 0.0f;
}

Camera::~Camera()
{
}

void Camera::processKeyBoardInputs(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camPos += camFront * camSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camPos -= camFront * camSpeed * deltaTime;;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed * deltaTime;;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed * deltaTime;;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camPos += camUp * camSpeed * deltaTime;;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camPos -= camUp * camSpeed * deltaTime;;
}

void Camera::processMouseMovement(GLFWwindow* window, double x, double y)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y;

	lastX = x;
	lastY = y;

	float sensitivity = 0.1f;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 dir;
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	dir.y = sin(glm::radians(pitch));
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront = glm::normalize(dir);
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(camPos, camPos + camFront, camUp);
}