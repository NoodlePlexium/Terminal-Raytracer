#pragma once
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <sstream>
#include <string>
#include <sstream>


namespace TerminalEngine {

struct Colour
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    Colour(uint8_t red=0, uint8_t green=0, uint8_t blue=0) : r(red), g(green), b(blue) {}

    bool operator==(const Colour& other) const {
        return (r == other.r && g == other.g && b == other.b);
    }

    bool operator!=(const Colour& other) const {
        return !(*this == other);
    }
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

    Window()
    {
        // Get the handle to the console output buffer
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole == INVALID_HANDLE_VALUE) {
            std::cerr << "Error: Could not get console handle." << std::endl;
            return;
        }

        Resize();

        // Set console output to UTF-8
        SetConsoleOutputCP(CP_UTF8);
        enableVirtualTerminalProcessing();
        system("cls");
    }

    void Render() {

        ClearScreen();

        std::ostringstream printStr;
        TerminalEngine::Colour currentTextColour = frameBuffer[0].textCol;
        TerminalEngine::Colour currentBgColour = frameBuffer[0].bgCol;
        printStr << TextColourEscapeCode(currentTextColour);
        printStr << BackgroundColourEscapeCode(currentBgColour);

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                int pixelIndex = y * WIDTH + x;
                if (currentTextColour != frameBuffer[pixelIndex].textCol)
                {
                    currentTextColour = frameBuffer[pixelIndex].textCol;
                    printStr << "\x1b[0m"; // End colour
                    printStr << TextColourEscapeCode(currentTextColour);
                    printStr << BackgroundColourEscapeCode(currentBgColour);
                }
                if (currentBgColour != frameBuffer[pixelIndex].bgCol)
                {
                    currentBgColour = frameBuffer[pixelIndex].bgCol; 
                    printStr << "\x1b[0m"; // End Colour
                    printStr << TextColourEscapeCode(currentTextColour);
                    printStr << BackgroundColourEscapeCode(currentBgColour);
                }
                printStr << frameBuffer[pixelIndex].ch;
            }
            printStr << "\n";
        }

        std::cout << printStr.str();
        std::cout.flush();


        Resize();
    } 

    void SetPixel(const unsigned int x, const unsigned int y, const char character, Colour textColour, Colour backgroundColour, bool textOnly=false)
    {
        int pixelIndex = y * WIDTH + x;
        if (!textOnly) frameBuffer[pixelIndex].bgCol = backgroundColour;
        frameBuffer[pixelIndex].textCol = textColour;
        frameBuffer[pixelIndex].ch = character;
    }

    unsigned int GetWidth() { return WIDTH; }
    unsigned int GetHeight() { return HEIGHT; }

    void ShowCursor(bool showFlag)
    {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = showFlag; 
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    void TextHighlighting(bool enableFlag) 
    {
        DWORD consoleMode;
        if (!GetConsoleMode(hConsole, &consoleMode)) {
            std::cerr << "Error: Could not get console mode." << std::endl;
            return;
        }

        if (enableFlag) {
            consoleMode |= ENABLE_QUICK_EDIT_MODE; // Enable text highlighting (Quick Edit mode)
        } else {
            consoleMode &= ~ENABLE_QUICK_EDIT_MODE; // Disable text highlighting (Quick Edit mode)
        }

        if (!SetConsoleMode(hConsole, consoleMode)) {
            std::cerr << "Error: Could not set console mode." << std::endl;
            return;
        }
    }

private:

    unsigned int WIDTH = 0;
    unsigned int HEIGHT = 0;
    unsigned int MIN_WIDTH = 0;
    unsigned int MIN_HEIGHT = 0;
    Pixel* frameBuffer;
    HANDLE hConsole;

    void CreateFrameBuffer()
    {
        unsigned int frameBufferSize = WIDTH * HEIGHT;
        frameBuffer = new Pixel[frameBufferSize];
    }

    void ClearScreen()
    {	
        COORD cursorPosition;	
        cursorPosition.X = 0;	
        cursorPosition.Y = HEIGHT;	
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
        std::cout << "\x1b[H";
    }

    void Resize()
    {
        // check for window resize
        int width, height;
        GetConsoleSize(width, height);

        if (width != WIDTH || height != HEIGHT)
        {
            WIDTH = width;
            HEIGHT = height;

            // constrain min width and height
            if (WIDTH < MIN_WIDTH || HEIGHT < MIN_HEIGHT)
            {
                WIDTH = MIN_WIDTH;
                HEIGHT = MIN_HEIGHT;

                // Set the new screen buffer size
                COORD newSize;
                newSize.X = MIN_WIDTH;
                newSize.Y = MIN_HEIGHT;
                if (!SetConsoleScreenBufferSize(hConsole, newSize)) {
                    std::cerr << "Error: Could not set console screen buffer size." << std::endl;
                    return;
                }

                // Define the new console window size
                SMALL_RECT newRect;
                newRect.Top = 0;
                newRect.Left = 0;
                newRect.Right = MIN_WIDTH - 1;
                newRect.Bottom = MIN_HEIGHT - 1;

                // Set the new console window size
                if (!SetConsoleWindowInfo(hConsole, TRUE, &newRect)) {
                    std::cerr << "Error: Could not set console window size." << std::endl;
                    return;
                }
            }
            CreateFrameBuffer();
        }
        std::cout << "\x1b[H";
    }

    void enableVirtualTerminalProcessing() 
    {
        DWORD dwMode = 0;
        GetConsoleMode(hConsole, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsole, dwMode);
    }

    void GetConsoleSize(int &width, int &height)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) 
        {
            width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            height = csbi.srWindow.Bottom - csbi.srWindow.Top;
        } 
        else 
        {
            std::cerr << "Error retrieving console screen buffer info." << std::endl;
            width = 0;
            height = 0;
        }
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
}
