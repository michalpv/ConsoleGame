#include <Windows.h>
#include <stdio.h>
#include "Frames.h"

#define ESC "\x1b"
#define cWidth 120
#define cHeight 30

char *screenBuff = NULL;
BOOL newFrame = TRUE;
int level = 0;

void DrawFrame() {
    if (newFrame) {
        printf(ESC "7"); // Save cursor position in memory
        printf(ESC "[0;0H"); // Move cursor to origin on console
		printf(ESC "[%uX", cWidth * cHeight); // Sequence for erasing <n> number of characters from cursor position
        printf("%s", screenBuff); // screenBuff treated as a 1D array
        printf(ESC "[0m"); // Reset colors
        printf(ESC "8"); // Restore cursor position from memory
        newFrame = FALSE;
    }
}

// Take buffer of characters (buff), draw line by line starting at X, Y
void AddFrameObject(char *buff, unsigned int width, unsigned int height, unsigned int X, unsigned int Y) {
    // Account for out of bounds objects in X axis
    if (X + width > cWidth) {
        width -= cWidth-X;
    }
    // Account for out of bounds objects in X axis
    if (Y + height > cHeight) {
        height -= cHeight-Y;
    }
    // Write to screenBuff
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            screenBuff[((Y+h)*cWidth)+(X+w)] = buff[(h*width)+w]; // Brain melting math
        }
    }
    newFrame = TRUE;
}

int ConsoleSetup(HANDLE hConIn, HANDLE hConOut) {
    COORD size;
    size.X = cWidth; // Set console buffer width and height to 120 x 30
    size.Y = cHeight;

    if (!SetConsoleScreenBufferSize(hConOut, size)) { // Set console buffer size
        return GetLastError();
    }

    // Set up input mode
    DWORD modeFlags = 0;
    if(!GetConsoleMode(hConIn, &modeFlags)) { // If function fails, return value is ZERO
        return GetLastError();
    }

    modeFlags = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS; // Add additional flags if necessary
	//https://stackoverflow.com/questions/23666035/how-to-make-readconsoleinput-notice-mouse-events-when-program-is-running-from
    if(!SetConsoleMode(hConIn, modeFlags)) { // Enable mouse input for console
        return GetLastError();
    }

    // Set up output
    if(!GetConsoleMode(hConOut, &modeFlags)) { // If function fails, return value is ZERO
        return GetLastError();
    }

    modeFlags |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if(!SetConsoleMode(hConOut, modeFlags)) { // Enable VT processing (colours, cursor positioning, etc.)
        return GetLastError();
    }
    return 0;
}

int main() {
    // Used to store all on-screen characters
    int screenSize = cWidth*cHeight + 1000; // + 1000 for null term + any escape characters, VT sequences etc.
    screenBuff = (char *) calloc(screenSize, sizeof(char)); // Allocate heap memory
    if (screenBuff == NULL) {
        printf("[-] Not enough memory; exiting.");
        return 1;
    }
    // Setup console
    HANDLE hConIn, hConOut;

    hConIn = GetStdHandle(STD_INPUT_HANDLE);
    hConOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hConIn == INVALID_HANDLE_VALUE || hConOut == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }

    int result = ConsoleSetup(hConIn, hConOut);
    if (result) { // ConsoleSetup unsuccessful, return and terminate
        return result;
    }

    char test[] =   "TEST"
                    "ESTE"
                    "STES"
                    "TEST";

    
    AddFrameObject(mainMenu, cWidth, cHeight, 0, 0);
    DrawFrame();
    Sleep(1000);
    AddFrameObject(test, 4, 4, 5, 20);
    DrawFrame();
    Sleep(1000);

    // Run game loop here:
    while(1) {
        AddFrameObject(starFrameA, cWidth, cHeight, 0, 0);
        DrawFrame();
        Sleep(1000);
        AddFrameObject(starFrameB, cWidth, cHeight, 0, 0);
        DrawFrame();
        Sleep(1000);
    }

    return 0;
}
