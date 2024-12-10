#include "Sphere.h"
#include "Camera.h"
#include "Ray.h"
#include "RayTracer.h"
#include "GCP_GFX_Framework.h"
#include <thread>

void Rendering(rayTracer& rayTracer, camera& camera, GCP_Framework& _MyFramework, int startY, int endY, glm::ivec2 winSize)
{
	for (int y = startY; y < endY; y++)
	{
		for (int x = 0; x < winSize.x; x++)
		{
			ray ray = camera.GetRay(glm::ivec2(x, y));
			glm::vec3 colour = rayTracer.TraceRay(ray);
			_MyFramework.DrawPixel(glm::ivec2(x, y), colour);
		}
	}
}

int main(int argc, char* argv[])
{
	// Set window size
	glm::ivec2 winSize(640, 480);

	// This will handle rendering to screen
	GCP_Framework _myFramework;

	// Initialises SDL and OpenGL and sets up a framebuffer
	if (!_myFramework.Init(winSize))
	{
		return -1;
	}

	rayTracer rayTracer;
	camera camera;
	ray Ray;

	sphere sphere1(glm::vec3(320.0f, 240.0f, -100.0f), 50.0f);
	sphere sphere2(glm::vec3(200.0f, 200.0f, -50.0f), 100.0f);
	rayTracer.AddSphere(&sphere1);
	rayTracer.AddSphere(&sphere2);

	//Get time before rendering
	std::chrono::steady_clock::time_point time1 = std::chrono::high_resolution_clock::now();

	const int threadCount = 4;
	const int iterations = 10;
	int chunkHeight = winSize.y / threadCount;
	std::vector<std::thread> threads;

	for (int i = 0; i < threadCount; ++i)
	{
		int startY = i * chunkHeight;
		int endY = (i == threadCount - 1) ? winSize.y : startY + chunkHeight;
		threads.emplace_back(Rendering, std::ref(rayTracer), std::ref(camera), std::ref(_myFramework), startY, endY, winSize);
		std::cout << "Thread " << i << " rendering rows " << startY << " to " << endY << std::endl;
	}
	for (int i = 0; i < threadCount; ++i)
	{
		threads[i].join();
	}

	//Basic rendering loop
	//for (int x = 0; x < winSize.x; x++)
	//{
	//	for (int y = 0; y < winSize.y; y++)
	//	{
	//		ray ray = camera.GetRay(glm::ivec2(x, y));
	//		glm::vec3 colour = rayTracer.TraceRay(ray);
	//		_myFramework.DrawPixel(glm::ivec2(x, y), colour);
	//	}
	//}

	//Get time and turn into milliseconds
	std::chrono::steady_clock::time_point time2 = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1);
	std::cout << "Time taken: " << milliseconds.count() << std::endl;



	//Pushes the framebuffer to OpenGL and renders to screen
	//Also contains an event loop that keeps the window going until it's closed
	_myFramework.ShowAndHold();
	return 0;


}
