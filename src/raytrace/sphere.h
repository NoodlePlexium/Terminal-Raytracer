#pragma once
#include <cmath>
#include "utils.h"
#include "material.h"
#include "vector.h"
#include "ray.h"

struct Sphere
{
    float radius;
    Material material;
    Vector3 position;

    Sphere(float _radius = 1.0f, const Vector3& _position = Vector3(), const Material& _material = Material()) : radius(_radius), position(_position), material(_material) {}
};

RayHit RaySphere(const Ray& ray, const Sphere& sphere)
{
    RayHit hit;
    Vector3 oc = ray.origin - sphere.position;
    float b = 2.0f * Vector3::Dot(oc, ray.direction);
    float c = Vector3::Dot(oc, oc) - (sphere.radius * sphere.radius);
    float discriminant = b * b - 4.0f * c;

    if (discriminant < 0.0f) {
        return hit;
    }

    float sqrtDiscriminant = std::sqrt(discriminant);

    // find the closest intersection distance
    float t1 = (-b - sqrtDiscriminant) / 2.0f;
    float t2 = (-b + sqrtDiscriminant) / 2.0f;


    if (t1 > 0.0f || t2 > 0.0f) 
    {
        float t = (t1 > 0.0f) ? t1 : t2;

        // calculate intersection position
        Vector3 hitPos = ray.origin + ray.direction * t;
        Vector3 normal = (hitPos - sphere.position).Normalized();
        return RayHit(ray, hitPos, t, normal, sphere.material, true);
    }
    return hit;
}