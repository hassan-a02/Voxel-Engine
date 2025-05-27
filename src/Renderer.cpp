#include "Renderer.h"
#include <memor

Renderer::Renderer() : camera()
{
	projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 1000.0f);
}

void Renderer::init(GLFWwindow*& window)
{
    glfwMakeContextCurrent(window);

    
    glfwSetWindowUserPointer(window, &camera);

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::cout << "Creating shader\n";
    try {
        shader = new Shader("shaders/vert.glsl", "shaders/frag.glsl");
    }
    catch (const std::exception& e) {
        std::cerr << "Shader creation failed: " << e.what() << "\n";
        return;
    }
    this->window = window;

    int width, height, nrChannels;
    BlockType curr;

    for (int i = 0; i < 4; i++) {
        unsigned char* data = nullptr;
        int width, height, nrChannels;

        switch (BlockType(i)) {
		case BlockType::Air:
			curr = BlockType::Air;
            continue;
        case BlockType::Grass:
            data = stbi_load("textures/grass.jpg", &width, &height, &nrChannels, 0);
            curr = BlockType::Grass;
            break;
        case BlockType::Dirt:
            data = stbi_load("textures/dirt.jpg", &width, &height, &nrChannels, 0);
            curr = BlockType::Dirt;
            break;
        case BlockType::Stone:
            data = stbi_load("textures/stone.jpg", &width, &height, &nrChannels, 0);
            curr = BlockType::Stone;
            break;
        }

        if (data == nullptr) {
            std::cout << "Failed to load texture " << (int)curr << ": " << stbi_failure_reason() << "\n";
            continue;
        }
        std::cout << "Loaded " << (int)curr << ": " << width << "x" << height << ", channels: " << nrChannels << "\n";


        unsigned int texture;
        glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        textures[curr] = texture;

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);

        glEnable(GL_DEPTH_TEST);
    }

    int samplers[4] = { 0, 1, 2, 3 };
    shader->use();
    glUniform1iv(glGetUniformLocation(shader->id, "blockTextures"), 4, samplers);
    shader->unbind();
}

void Renderer::drawChunks(std::vector<std::shared_ptr<Chunk>>& chunks)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;


    camera.processKeyBoardInputs(window, deltaTime);

	for (auto& chunk : chunks)
	{
		model = glm::mat4(1.0f);
        model = glm::translate(model, chunk->getWorldPosition());
        view = camera.getViewMatrix();

        shader->use();

        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        for (int i = 0; i < 4; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[(BlockType)i]);
        }

	    glBindVertexArray(chunk->mesh.vao);

	    glDrawElements(GL_TRIANGLES, chunk->mesh.indices.size(), GL_UNSIGNED_INT, 0);

	    glBindVertexArray(0);
	    glBindTexture(GL_TEXTURE_2D, 0);
	}

    

	std::cout << "Cam pos: " << camera.getPos().x << ", " << camera.getPos().y << ", " << camera.getPos().z << "\n";
}

void Renderer::uploadChunkMesh(ChunkMesh& mesh)
{
    glGenVertexArrays(1, &mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.veo);



    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.veo);

    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::vector<std::shared_ptr<Chunk>> Renderer::getNeighbours(std::vector<std::shared_ptr<Chunk>> chunks, std::shared_ptr<Chunk> baseChunk)
{
    std::vector<std::shared_ptr<Chunk>> neighbours;
    neighbours.reserve(4);

    std::shared_ptr<Chunk> negX;
    std::shared_ptr<Chunk> posX;
    std::shared_ptr<Chunk> negZ;
    std::shared_ptr<Chunk> posZ;


    for (int i = 0; i < chunks.size(); i++)
    {
        if (chunks[i]->isValid())
        {
            if (chunks[i]->getChunkCoord() == ChunkCoord(baseChunk->getChunkCoord().x - 1, baseChunk->getChunkCoord().z))
                negX = chunks[i];
            else if (chunks[i]->getChunkCoord() == ChunkCoord(baseChunk->getChunkCoord().x + 1, baseChunk->getChunkCoord().z))
                posX = chunks[i];
            else if (chunks[i]->getChunkCoord() == ChunkCoord(baseChunk->getChunkCoord().x, baseChunk->getChunkCoord().z - 1))
                negZ = chunks[i];
            else if (chunks[i]->getChunkCoord() == ChunkCoord(baseChunk->getChunkCoord().x, baseChunk->getChunkCoord().z + 1))
                posZ = chunks[i];
        }
    }


    neighbours.push_back(negX);
    neighbours.push_back(posX);
    neighbours.push_back(negZ);
    neighbours.push_back(posZ);


    return neighbours;
}


void Renderer::mouseCallback(GLFWwindow* window, double x, double y)
{
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    if (camera) camera->processMouseMovement(window, x, y);
}

