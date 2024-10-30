#pragma once
#include <cmath>
#include <iostream>

struct Vector3
{
    float x;
    float y;
    float z;

    // Default constructor
    Vector3() : x(0), y(0), z(0) {}

    // Constructor taking in rgb values
    Vector3(float xVal, float yVal, float zVal) : x(xVal), y(yVal), z(zVal) {}

    // assignment operator
    Vector3& operator=(const Vector3& other)
    {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }
    
    // Dot product inplace
    float Dot(const Vector3 &vec) const
    {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    // Dot product of two vectors
    static float Dot(const Vector3 &vec1, const Vector3 &vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }    

    // Cross product in place
    void Cross(const Vector3 &vec)
    {
        x = y * vec.z - z * vec.y;
        y = z * vec.x - x * vec.z;
        z = x * vec.y - y * vec.x;
    }

    // Cross product of two vectors
    static Vector3 Cross(const Vector3 &vec1, const Vector3 &vec2)
    {
        Vector3 result;
        result.x = vec1.y * vec2.z - vec1.z * vec2.y;
        result.y = vec1.z * vec2.x - vec1.x * vec2.z;
        result.z = vec1.x * vec2.y - vec1.y * vec2.x;
        return result;
    }

    Vector3 Normalized() const {
        double mag = Magnitude();
        if (mag == 0.0f) 
        {
            std::cerr << "[Vector::Normalize] ERROR! cannot normalize a zero vector!" << "\n";
            return Vector3();
        }
        return Vector3(x / mag, y / mag, z / mag);
    }

    // static add
    static Vector3 Add(const Vector3 &vec1, const Vector3 &vec2)
    {
        Vector3 result;
        result.x = vec1.x + vec2.x;
        result.y = vec1.y + vec2.y;
        result.z = vec1.z + vec2.z;
        return result;
    }

    Vector3 operator+(const Vector3 &other) const
    {
        float newX = x + other.x;
        float newY = y + other.y;
        float newZ = z + other.z;
        return Vector3(newX, newY, newZ);
    }

    Vector3 operator-(const Vector3 &other) const
    {
        float newX = x - other.x;
        float newY = y - other.y;
        float newZ = z - other.z;
        return Vector3(newX, newY, newZ);
    }

    // static subtract
    static Vector3 Subtract(const Vector3 &vec1, const Vector3 &vec2)
    {
        Vector3 result;
        result.x = vec1.x - vec2.x;
        result.y = vec1.y - vec2.y;
        result.z = vec1.z - vec2.z;
        return result;
    }

    Vector3 operator*(const Vector3 &other) const
    {
        float newX = x * other.x;
        float newY = y * other.y;
        float newZ = z * other.z;
        return Vector3(newX, newY, newZ);
    }

    Vector3 operator*(float val) const
    {
        float newX = x * val;
        float newY = y * val;
        float newZ = z * val;
        return Vector3(newX, newY, newZ);
    }

    // inplace version
    void Divide(const Vector3 &vec)
    {
        if (vec.x == 0.0f || vec.y == 0.0f || vec.z == 0.0f) 
        {
            std::cerr << "[Vector::Divide] ERROR! cannot divide by zero!" << "\n";
            return;
        }
        x /= vec.x;
        y /= vec.y;
        z /= vec.z;
    }

    // static version
    static Vector3 Divide(const Vector3 &vec1, const Vector3 &vec2)
    {
        if (vec2.x == 0.0f || vec2.y == 0.0f || vec2.z == 0.0f) 
        {
            std::cerr << "[Vector::Divide] ERROR! cannot divide by zero!" << "\n";
            Vector3 returnVec;
            return returnVec;
        }
        Vector3 result;
        result.x = vec1.x / vec2.x;
        result.y = vec1.y / vec2.y;
        result.z = vec1.z / vec2.z;
        return result;
    }

    // static
    static Vector3 ScalarMult(const Vector3 &vec1, float value)
    {
        Vector3 result;
        result.x = vec1.x * value;
        result.y = vec1.y * value;
        result.z = vec1.z * value;
        return result;
    }
    
    float Magnitude() const
    {
        return sqrtf(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    float DistFrom(const Vector3 &vec) const
    {
        return sqrtf(pow(x - vec.x, 2) + pow(y - vec.y, 2) + pow(z - vec.z, 2));
    }

    static float Dist(const Vector3 &vec1, const Vector3 &vec2) 
    {
        return sqrtf(pow(vec1.x - vec2.x, 2) + pow(vec1.y - vec2.y, 2) + pow(vec1.z - vec2.z, 2));
    }
};