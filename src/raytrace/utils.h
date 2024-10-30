#pragma once
#include "vector.h"
#include "colour.h"
#include <iostream>
#include <string>
#include <random>

namespace Utility {

float Random(float min, float max) {
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
}

float RandomValueNormalDistribution()
{
    float theta = 2 * 3.1415926 * Random(0, 1);
    float rho = std::sqrt(-2 * std::log(Random(0, 1)));
    return rho * std::cos(theta);
}

Vector3 RandomDirection(){
    float x = RandomValueNormalDistribution();
    float y = RandomValueNormalDistribution();
    float z = RandomValueNormalDistribution();
    Vector3 randomDir(x,y,z);
    return randomDir.Normalized();
}

Vector3 RandomHemisphereDirection(const Vector3& normal) 
{

    float x = RandomValueNormalDistribution();
    float y = RandomValueNormalDistribution();
    float z = RandomValueNormalDistribution();

    Vector3 randomDirection = RandomDirection();

    if (randomDirection.Dot(normal) < 0.0f){
        randomDirection = randomDirection * -1.0f;
    }

    return randomDirection;
}

void PrintVector(std::string label, const Vector3 vec)
{
    std::cout << label << ": " << vec.x << " " << vec.y << " " << vec.z << "\n";
}

void PrintFloat(std::string label, const float val)
{
    std::cout << label << ": " << val << "\n";
}

void PrintColour(std::string label, const Colour col)
{
    std::cout << label << ": " << col.red << " " << col.green << " " << col.blue << "\n";
}

inline int Min(int a, int b) {
    return (a < b) ? a : b;
}
}


