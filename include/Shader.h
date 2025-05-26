#pragma once

#include "glad/glad.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int id;

	Shader(const char* vertPath, const char* fragPath);

	void use();
	void unbind();

	void setMat4(const std::string& name, const glm::mat4& mat);
	void setInt(const std::string& name, const int& integer);
	void setIntArray(const std::string& name, const int& integer);
};

