#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include "Frames.h"

#define ESC "\x1b"
#define cWidth 120
#define cHeight 30

char *screenBuff = NULL;
BOOL newFrame = TRUE;

// Only draw frame if new frame is set
void DrawFrame() {
    printf(ESC "7"); // Save cursor position in memory
    printf(ESC "[0;0H"); // Move cursor to origin on console
    printf(ESC "[%uX", cWidth * cHeight); // Sequence for erasing <n> number of characters from cursor position
    printf("%s", screenBuff); // screenBuff treated as a 1D array
    printf(ESC "[0m"); // Reset colors
    printf(ESC "8"); // Restore cursor position from memory
    newFrame = FALSE;
}

// Will return Win32 defined datatype by pointer with mouse event information
BOOL GetMouseInputRecord(HANDLE hConIn, MOUSE_EVENT_RECORD *mouseRecord) {
    DWORD eventsAvailable = 0;
    GetNumberOfConsoleInputEvents(hConIn, &eventsAvailable);
    while (eventsAvailable > 0) {
        INPUT_RECORD inRecord;
        DWORD eventsRead;
        if (ReadConsoleInput(hConIn, &inRecord, 1, &eventsRead)) {
            if (inRecord.EventType == MOUSE_EVENT) {
                memcpy(mouseRecord, &inRecord.Event.MouseEvent, sizeof(MOUSE_EVENT_RECORD));
            }
            eventsAvailable--;
        }
        else {
            return FALSE;
        }
    }
    return TRUE;
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

BOOL isClicked(xM, yM, width, height, xB, yB) { // Keep in mind that origin cell starts at 0, therefore mouse position ranges from 0
    if (xB < xM && xM < xB + width && yB < yM && yB + height)
        return TRUE;
    else
        return FALSE;
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
    
    // Initalization:
    AddFrameObject(mainMenu, cWidth, cHeight, 0, 0);
    DrawFrame();

    int level = 0; // Start at main menu
    // starFrame init:
    short sceneFlipper = 1;

    // Run game loop here:
    clock_t t = clock();// Get elapsed time
    while(1) {
        switch (level) {
            case 0: // Main menu
                MOUSE_EVENT_RECORD mouseRecord;
                if (GetMouseInputRecord(hConIn, &mouseRecord)) {
                    if (mouseRecord.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) { // Check that left-most button is pressed
                        // Check that START button is pressed using positional function
                        if (isClicked(
                            mouseRecord.dwMousePosition.X,
                            mouseRecord.dwMousePosition.Y,
                            34, 6, 43, 23 // Start button location
                            )) { // bool isClicked(xM, yM, width, height, xB, yB)
                            level = 1;
                            t = clock(); // Reset clock after changing level
                        }
                    }
                }
                break;
            case 1: // starFrame
                if ((double)clock() - (double)t/CLOCKS_PER_SEC > 10) {
                    sceneFlipper *= -1;
                    t = clock();
                }

                if (sceneFlipper == 1) {
                    AddFrameObject(starFrameA, cWidth, cHeight, 0, 0); // Change entire buffer
                }
                else if(sceneFlipper == -1) {
                    AddFrameObject(starFrameB, cWidth, cHeight, 0, 0); // Change entire buffer
                }
                break;
        }
        if (newFrame) {
            DrawFrame();
        }
    }

    return 0;
}
