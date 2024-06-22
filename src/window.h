#pragma once
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <sstream>
#include <string>



struct Colour
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct Pixel
{
    Colour bgCol;
    Colour textCol;
    char ch;
};


class Window
{
public:

    Window(unsigned int width, unsigned int height) : WIDTH(width), HEIGHT(height)
    {
        CreateFrameBuffer();

        // Set console output to UTF-8
        SetConsoleOutputCP(CP_UTF8);
        enableVirtualTerminalProcessing();
        system("cls");
    }

    void Render() {

        ClearScreen();

        // Preallocate memory
        std::string printStr;
        printStr.reserve(WIDTH * HEIGHT * 12); 

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                int pixelIndex = y * WIDTH + x;
                printStr += TextColourEscapeCode(frameBuffer[pixelIndex].textCol);
                printStr += BackgroundColourEscapeCode(frameBuffer[pixelIndex].bgCol);
                printStr += frameBuffer[pixelIndex].ch;
                printStr += "\x1b[0m"; // Reset colors
            }
            printStr += "\n";
        }

        std::cout << printStr;
        std::cout.flush();
    } 

    void SetPixel(const unsigned int x, const unsigned int y, const char character, Colour textColour, Colour backgroundColour)
    {
        int pixelIndex = y * WIDTH + x;

        frameBuffer[pixelIndex].bgCol = backgroundColour;
        frameBuffer[pixelIndex].textCol = textColour;
        frameBuffer[pixelIndex].ch = character;
    }

    void UpdateDimensions(unsigned int width, unsigned int height)
    {
        WIDTH = width;
        HEIGHT = height;
        CreateFrameBuffer();
    }

    unsigned int GetWidth() { return WIDTH; }
    unsigned int GetHeight() { return HEIGHT; }

private:

    unsigned int WIDTH = 30;
    unsigned int HEIGHT = 20;
    Pixel* frameBuffer;

    void CreateFrameBuffer()
    {
        unsigned int frameBufferSize = WIDTH * HEIGHT;
        frameBuffer = new Pixel[frameBufferSize];
    }

    void ClearScreen()
    {	
        COORD cursorPosition;	
        cursorPosition.X = 0;	
        cursorPosition.Y = 0;	
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
        std::cout << "\x1b[H";
    }

    void enableVirtualTerminalProcessing() 
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    std::string TextColourEscapeCode(const Colour& colour)
    {
        std::string colourString = "\x1b[38;2;" + std::to_string(colour.r) + 
                    ";" + std::to_string(colour.g) + 
                    ";" + std::to_string(colour.b) + "m";
        return colourString;
    }

    std::string BackgroundColourEscapeCode(const Colour& colour)
    {
        std::string colourString = "\x1b[48;2;" + std::to_string(colour.r) + 
                    ";" + std::to_string(colour.g) + 
                    ";" + std::to_string(colour.b) + "m";
        return colourString;
    }
};