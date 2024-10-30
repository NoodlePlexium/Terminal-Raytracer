#include <iostream>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>
#include <omp.h>
#include <chrono>
#include "terminal/window.h"
#include "terminal/gui.h"
#include "raytrace/raytracer.h"




int main()
{
    TerminalEngine::Window window{};
    window.ShowCursor(false);
    double frameTime;

    // create scene objects
    std::vector<Sphere> spheres;
    Material mat1(1, 0, 0, Colour(1, 0, 0, 1));
    Material mat2(1, 0, 0, Colour(0, 1, 0, 1));
    Material mat3(1, 0, 0, Colour(0, 0, 1, 1));
    Material mat4(1, 0, 0, Colour(1, 1, 1, 1));
    Sphere sphere1(1.0f, Vector3(-2, 0, 4), mat1);
    Sphere sphere2(1.0f, Vector3(0, 0, 4), mat2);
    Sphere sphere3(1.0f, Vector3(2, 0, 4), mat3);
    Sphere sphere4(100.0f, Vector3(0, 101.0f, 4), mat4);
    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);
    spheres.push_back(sphere4);

    while (true) 
    {      
        auto start = std::chrono::high_resolution_clock::now();


        RayTrace(window, spheres, 8, 100);


        // display frame time
        std::string framerateString = "[Frame Time] " + std::to_string(frameTime) + " ms";
        TerminalEngine::DrawText(framerateString, 2, 1, TerminalEngine::Colour(255, 255, 255), window);
    

        window.Render();
        

        // calculate frame time
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        frameTime = static_cast<double>(duration.count());
    }
    return 0;
}