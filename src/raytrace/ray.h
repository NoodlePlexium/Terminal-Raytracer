#pragma once
#include "vector.h"
#include "material.h"
#include "colour.h"

struct Ray
{
    Vector3 origin, direction;
    Ray(const Vector3& _origin, const Vector3& _direction) : origin(_origin), direction(_direction) {}
};

struct RayHit
{
    Ray ray;
    Vector3 hitPosition;
    Vector3 hitNormal;
    float hitDist;
    Material material;
    bool didHit;

    // constructor
    RayHit(
        Ray _ray = Ray(Vector3(), Vector3()), 
        Vector3 _hitPos = Vector3(), 
        float _hitDist = std::numeric_limits<float>::infinity(),
        Vector3 _hitNormal = Vector3(),
        const Material _material = Material(),
        bool _didHit = false)
    : 
        ray(_ray), 
        hitPosition(_hitPos), 
        hitDist(_hitDist), 
        hitNormal(_hitNormal), 
        material(_material), 
        didHit(_didHit) {}

};