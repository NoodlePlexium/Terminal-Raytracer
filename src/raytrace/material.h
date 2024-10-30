#pragma once
#include "colour.h"

struct Material {
    double roughness;
    double metalness;
    double emissionStrength;
    Colour colour;

    Material(double _roughness = 0.5, double _metalness = 0, double _emissionStrength = 0, const Colour& _colour = Colour(0.6f, 0.6f, 0.6f, 1.0f))
        : roughness(_roughness), metalness(_metalness), emissionStrength(_emissionStrength), colour(_colour) {}
};