#pragma once
#include <omp.h>
#include "../terminal/window.h"
#include "../terminal/gui.h"
#include "vector.h"
#include "sphere.h"
#include "material.h"
#include "colour.h"
#include "utils.h"

Vector3 DirFromPixel(int x, int y, int width, int height)
{
    // camera settings
    float fov = 50.0f;
    float aspectRatio = width / static_cast<float>(height);
    float pixelInverseAspectRatio = 1.6f;
    float nearPlane = 0.1f;
    Vector3 camForward(0.0f, 0.0f, 1.0f);
    Vector3 camRight(1.0f, 0.0f, 0.0f);
    Vector3 camUp(0.0f, 1.0f, 0.0f);
    Vector3 camPosition(0.0f, 0.0f, 0.0f);

    // projection plane
    float planeHeight = nearPlane * std::tan(fov * 0.5f * 0.01745329f) * pixelInverseAspectRatio;
    float planeWidth = planeHeight * aspectRatio / 2;

    // convert screen width & height to range (0, 1)
    float tx = x / (width - 1.0f);
    float ty = y / (height - 1.0f);

    // calculate direction
    Vector3 bottomLeftLocal(-planeWidth / 2, -planeHeight / 2, nearPlane);
    Vector3 pointLocal = bottomLeftLocal + Vector3(planeWidth * tx, planeHeight * ty, 0.0f);
    Vector3 point = camPosition + camRight * pointLocal.x + camUp * pointLocal.y + camForward * pointLocal.z;
    Vector3 dir = (point - camPosition).Normalized();
    return dir;
}

RayHit CastRay(const Ray& ray, const std::vector<Sphere>& spheres)
{
    RayHit hit;
    for (int j=0; j<spheres.size(); ++j)
    {
        RayHit newHit = RaySphere(ray, spheres[j]);
        if (newHit.hitDist < hit.hitDist) hit = newHit;
    }
    return hit;
}

float ToneMap(float value, float key, float whitePoint, float exposure) {
    value *= exposure;
    value /= key;
    value = (value * (1.0f + value / (whitePoint * whitePoint))) / (1.0f + value);
    return value;
}

void RayTrace(TerminalEngine::Window& window, const std::vector<Sphere>& spheres, int bounces=3, int samples=100)
{
    # pragma omp parallel for
    for (unsigned int y=0; y<window.GetHeight(); ++y)
    {
        for (unsigned int x=0; x<window.GetWidth(); ++x)
        {
            Colour pixelColour;
            Colour skyColour(0.62f, 0.84f, 0.93f, 1.0f);
            Vector3 pixelDir = DirFromPixel(x, y, window.GetWidth(), window.GetHeight());


            Colour totalIncommingLight;
            for (int s=0; s<samples; ++s)
            {
                Colour incommingLight(0, 0, 0, 1.0f);
                Colour rayColour(1.0f, 1.0f, 1.0f, 1.0f);

                Ray ray(Vector3(), pixelDir);
                RayHit hit;

                for (int i=0; i<bounces+1; ++i)
                {
                    hit = CastRay(ray, spheres);
                    if (hit.didHit) // ray hit something
                    {
                        // prepare ray for next bounce
                        Vector3 diffuseDir = (hit.hitNormal + Utility::RandomDirection()).Normalized();
                        Vector3 specularDir = ray.direction - hit.hitNormal * 2.0f * Vector3::Dot(ray.direction, hit.hitNormal);
                        ray.direction = (diffuseDir * hit.material.roughness + specularDir * (1-hit.material.roughness)).Normalized();
                        ray.origin = hit.hitPosition + (hit.hitNormal * 0.0001f);

                        // calculate light contribution
                        Colour emmitedLight = hit.material.colour * hit.material.emissionStrength;
                        incommingLight += emmitedLight * rayColour * rayColour;
                        rayColour.blendAdd(hit.material.colour);
                    }
                    else // ray hit nothing
                    {
                        incommingLight += skyColour * skyColour * 3 * rayColour * rayColour;
                        break;
                    }
                }
                totalIncommingLight += incommingLight;
            }

            pixelColour = totalIncommingLight / samples;


            TerminalEngine::Colour windowPixelColour;
            windowPixelColour.r = static_cast<int>(ToneMap(pixelColour.red, 1, 500, 1) * 255);
            windowPixelColour.g = static_cast<int>(ToneMap(pixelColour.green, 1, 500, 1) * 255);
            windowPixelColour.b = static_cast<int>(ToneMap(pixelColour.blue, 1, 500, 1) * 255);
            window.SetPixel(x, y, ' ', windowPixelColour, windowPixelColour);
        }
    }
}
