#pragma once
#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "stb_image.h"

#include <map>

enum class BlockType
{
    Air,
    Grass,
    Dirt,
    Stone
};

class Block {
public:
	Block() = default;

    glm::vec3 position;
    BlockType blockType;

    Block(glm::vec3 startingPos, BlockType blockType);
};
