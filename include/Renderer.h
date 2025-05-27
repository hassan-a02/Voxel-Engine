#pragma once
#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "Block.h"
#include "Chunk.h"

#include <vector>
#include <memory>

static std::map<BlockType, GLuint> textures;


class Renderer
{
public:
	Renderer();


	void init(GLFWwindow*& window);


	void drawChunks(std::vector<std::shared_ptr<Chunk>>& chunks);
	void uploadChunkMesh(ChunkMesh& mesh);

	std::vector<std::shared_ptr<Chunk>> getNeighbours(std::vector<std::shared_ptr<Chunk>> chunks, std::shared_ptr<Chunk> baseChunk);

private:
	GLFWwindow* window;
	Shader* shader;
	Camera camera;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	float deltaTime;
	float lastFrame;

	static void mouseCallback(GLFWwindow* window, double x, double y);
};