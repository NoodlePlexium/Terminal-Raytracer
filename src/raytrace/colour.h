#pragma once

struct Colour {
    float red, green, blue, alpha;

    Colour(float _red = 0.0f, float _green = 0.0f, float _blue = 0.0f, float _alpha = 1.0f)
        : red(_red), green(_green), blue(_blue), alpha(_alpha) {}

    float Magnitude()
    {
        float magnitude = std::sqrtf(red * red + green * green + blue * blue);
        red /= magnitude;
        green /= magnitude;
        blue /= magnitude;
        return magnitude;
    }

    // Multiply the colour by a scalar value
    Colour operator*(float scalar) const {
        float newRed = red * scalar;
        float newGreen = green * scalar;
        float newBlue = blue * scalar;
        float newAlpha = alpha * scalar;
        return Colour(newRed, newGreen, newBlue, newAlpha);
    }

    // Multiply two colours component-wise
    Colour operator*(const Colour& other) const {
        float newRed = red * other.red;
        float newGreen = green * other.green;
        float newBlue = blue * other.blue;
        float newAlpha = alpha * other.alpha;
        return Colour(newRed, newGreen, newBlue, newAlpha);
    }

    // Add two colours
    Colour operator+(const Colour& other) const {
        float newRed = red + other.red;
        float newGreen = green + other.green;
        float newBlue = blue + other.blue;
        float newAlpha = alpha + other.alpha;
        return Colour(newRed, newGreen, newBlue, newAlpha);
    }

    // Subtract two colors
    Colour operator-(const Colour& other) const {
        float newRed = red - other.red;
        float newGreen = green - other.green;
        float newBlue = blue - other.blue;
        float newAlpha = alpha - other.alpha;
        return Colour(newRed, newGreen, newBlue, newAlpha);
    }

    // Divide the colour by a scalar value
    Colour operator/(float scalar) const {
        if (scalar != 0.0f) {
            float invScalar = 1.0f / scalar;
            float newRed = red * invScalar;
            float newGreen = green * invScalar;
            float newBlue = blue * invScalar;
            float newAlpha = alpha * invScalar;
            return Colour(newRed, newGreen, newBlue, newAlpha);
        }
        // Division by zero, return a black colour
        return Colour();
    }

    // Add two colors and update the current colour
    Colour& operator+=(const Colour& other) {
        red += other.red;
        green += other.green;
        blue += other.blue;
        alpha += other.alpha;
        return *this;
    }

    // Multiply the color by another colour and update the current color
    Colour& operator*=(const Colour& other) {
        red *= other.red;
        green *= other.green;
        blue *= other.blue;
        alpha *= other.alpha;
        return *this;
    }

    // Add two colors and update the current colour
    void blendAdd(const Colour& other) {
        alpha += other.alpha;
        red   = (red + other.red)     /2.0f;
        green = (green + other.green) /2.0f;
        blue  = (blue + other.blue)   /2.0f;
    }

    // Multiply the color by another colour and update the current color
    void blendMultiply(const Colour& other) {
        red *= other.red;
        green *= other.green;
        blue *= other.blue;
        alpha *= other.alpha;
    }
};