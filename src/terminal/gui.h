#pragma once

#include "window.h"
#include <string>

namespace TerminalEngine {

void DrawBorder(Window& window, int x, int y, int width, int height, const char character, const Colour &textColour, const Colour &backgroundColour)
{
    // top and bottom borders
    for (int i = x; i < x + width; ++i) {
        window.SetPixel(i, y, character, textColour, backgroundColour);
        window.SetPixel(i, y + height-1, character, textColour, backgroundColour);
    }

    // left and right borders
    for (int i = y; i < y + height; ++i) {
        window.SetPixel(x, i, character, textColour, backgroundColour);
        window.SetPixel(x + width-1, i, character, textColour, backgroundColour);
    }
}

void DrawText(std::string text, int x, int y, const Colour& colour, Window& window)
{
    for (int i=0; i<text.size(); ++i)
    {
        window.SetPixel(x+i, y, text[i], colour, Colour(), true);
    }
}
};
