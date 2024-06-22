#include <iostream>
#include <cstdint>
#include <sstream>
#include <string>
#include "window.h"




int main()
{


    Window window{100, 40};



    for (unsigned int y=0; y<window.GetHeight(); ++y)
    {
        for (unsigned int x=0; x<window.GetWidth(); ++x)
        {
            window.SetPixel(x, y, 'K', {30, 140, 20}, {0, 255, 0});
        }
    }

    while (true) {
        window.Render();
    }
}