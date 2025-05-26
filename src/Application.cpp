#include "Application.h"

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
}

Application::Application() : window(nullptr), renderer()
{
	init();
}


Application::~Application()
{
	cleanup();
}

void Application::run()
{
	std::vector<std::shared_ptr<Chunk>> chunks;

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			ChunkCoord coord(i, j);
			std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(coord);

			chunk->generateChunk();
			chunks.push_back(chunk);
		}
	}

	for (int i = 0; i < chunks.size(); i++)
	{
		std::vector<std::shared_ptr<Chunk>> neighbours = renderer.getNeighbours(chunks, chunks[i]);
		chunks[i]->generateChunkMesh(neighbours[0], neighbours[1], neighbours[2], neighbours[3]);
		renderer.uploadChunkMesh(chunks[i]->mesh);
	}


	while (!glfwWindowShouldClose(window))
	{
		renderer.drawChunks(chunks);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::init()
{
   if (!glfwInit())
       std::cout << "glfw init failed" << "\n";

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   window = glfwCreateWindow(1920, 1080, "Cube", nullptr, nullptr);

   if (window == nullptr)
	   std::cout << "window init failed" << "\n";

   glfwSetFramebufferSizeCallback(window, windowResize);

   glfwMakeContextCurrent(window);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	   std::cout << "glad init failed" << "\n";

   

   renderer.init(window);
}

void Application::cleanup()
{
	if (window)
	{
		glfwDestroyWindow(window);
		window = nullptr;
	}

	glfwTerminate();
}

void Application::windowResize(GLFWwindow* window, int x, int y)
{
	glViewport(0, 0, x, y);
}