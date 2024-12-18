#include "Sphere.h"
#include "Camera.h"
#include "Ray.h"
#include "RayTracer.h"
#include "GCP_GFX_Framework.h"
#include <thread>
#include <ctime>
#include <cstdlib>

struct TestResult 
{
    int milliseconds; 
    int sphereCount;   
    int threadCount;   
};

void writeResultsToCSV(const std::string& filename, const std::vector<TestResult>& results, bool append = false) {
    std::ofstream outFile;

    if (append) {
        outFile.open(filename, std::ios::out | std::ios::app);
    }
    else {
        outFile.open(filename, std::ios::out | std::ios::trunc);
    }

    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing!" << std::endl;
        return;
    }

    if (!append) {
        outFile << "Milliseconds,Sphere Count,Thread Count\n";
    }

    for (const auto& result : results) {
        outFile << result.milliseconds << ","
                << result.sphereCount << ","
                << result.threadCount << "\n";
    }

    outFile.close();
    std::cout << "Results successfully written to " << filename << std::endl;
}

glm::vec3 jitterSSAA(int samples, int x, int y, camera& camera, rayTracer& rayTracer)
{
    glm::vec3 accumulatedColour(0.0f);
    for (int i = 0; i < samples; i++)
    {
        float jitterX = (rand() / (float)RAND_MAX) - 0.5f;
        float jitterY = (rand() / (float)RAND_MAX) - 0.5f;
        glm::vec2 jitteredPos(x + jitterX, y + jitterY);
        ray jitteredRay = camera.GetRay(jitteredPos);
        glm::vec3 colour = rayTracer.TraceRay(jitteredRay);
        accumulatedColour += colour;
    }
    return accumulatedColour;
}

void Rendering(rayTracer& rayTracer, camera& camera, GCP_Framework& _MyFramework, int startY, int endY, glm::ivec2 winSize, bool AAtype)
{
    const int samples = 12;
    srand(static_cast<unsigned int>(time(0)));

    for (int y = startY; y < endY; y++)
    {
        for (int x = 0; x < winSize.x; x++)
        {
            if (AAtype = 1)
            {
                jitterSSAA(samples, x, y, camera, rayTracer);
                glm::vec3 accumulatedColour = jitterSSAA(samples, x, y, camera, rayTracer);
                glm::vec3 finalColour = accumulatedColour / static_cast<float>(samples);
                _MyFramework.DrawPixel(glm::ivec2(x, y), finalColour);
            }
            else
            {
                ray ray = camera.GetRay(glm::ivec2(x, y));
                glm::vec3 colour = rayTracer.TraceRay(ray);
                _MyFramework.DrawPixel(glm::ivec2(x, y), colour);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    //Set window size
    glm::ivec2 winSize(900, 900);

    //This will handle rendering to screen
    GCP_Framework _myFramework;

    //Initialises SDL and OpenGL and sets up a framebuffer
    if (!_myFramework.Init(winSize))
    {
        return -1;
    }

    rayTracer rayTracer;
    camera camera;
    ray Ray;

    //Add spheres to the screen based on sphereCount
    int sphereCount = 1;
    for (int s = 0; s < sphereCount; s++)
    {
        std::srand(std::time(nullptr));
        for (int i = 0; i < sphereCount; i++)
        {
            float x = rand() % winSize.x;
            float y = rand() % winSize.y;
            float z = -rand() % 1000;
            float radius = rand() % 100;
            sphere* sphere = new class sphere(glm::vec3(x, y, z), radius);
            rayTracer.AddSphere(sphere);
        }

        std::vector<TestResult> testResults;

        bool AAtype = 0; //AA type 1 = Jittered SSAA, 0 = no SSAA
        int threadCount = 16;
        //const int iterations = 1000; //number of tests ran
        int chunkHeight = winSize.y / threadCount;
        std::vector<std::thread> threads;

        //Get time before rendering
        std::chrono::steady_clock::time_point time1 = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < threadCount; ++i)
        {
            int startY = i * chunkHeight;
            int endY = (i == threadCount - 1) ? winSize.y : startY + chunkHeight;
            threads.emplace_back(Rendering, std::ref(rayTracer), std::ref(camera), std::ref(_myFramework), startY, endY, winSize, AAtype);
            std::cout << "Thread " << i << " rendering rows " << startY << " to " << endY << std::endl;
        }

        for (int i = 0; i < threadCount; ++i)
        {
            threads[i].join();
        }

        for (std::thread& t : threads) 
        {
            if (t.joinable())
            {
                t.join();
            }
        }

        //Get time and turn into milliseconds
        std::chrono::steady_clock::time_point time2 = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1);

        std::cout << "Time taken: " << milliseconds.count() << std::endl;
        std::cout << "Number of Spheres: " << sphereCount << std::endl;
        std::cout << "Number of Threads: " << threadCount << std::endl;

        int timeTaken = milliseconds.count();
        testResults.push_back({ timeTaken, sphereCount, threadCount });

        threads.clear();
        rayTracer.ClearSpheres();
        //Write all test results to CSV
		writeResultsToCSV("testResults.csv", testResults, true);
        sphereCount++;
        //Run the program in either hold or test mode
        _myFramework.Test(sphereCount);
        //_myFramework.ShowAndHold();
        if (sphereCount > 500)
        {
            return 0;
        }
        std::cout << "Sphere count increased to: " << sphereCount << std::endl;
    }

    return 0;
}
