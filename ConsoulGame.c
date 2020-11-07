#include <stdio.h>
#include <windows.h>

#define ESC "\x1b"

int ConsoleSetup(HANDLE hConIn, HANDLE hConOut) {
    COORD size;
    size.X = 120; // Set console window width and height to 120 x 30
    size.Y = 30;

    if (!SetConsoleScreenBufferSize(hConOut, size)) { // Set console buffer size
        return GetLastError();
    }

    DWORD modeFlags = 0;
    if(!GetConsoleMode(hConIn, &modeFlags)) { // If function fails, return value is ZERO
        return GetLastError();
    }

    modeFlags |= ENABLE_MOUSE_INPUT;
    if(!SetConsoleMode(hConIn, modeFlags)) { // Enable mouse input for console
        return GetLastError();
    }

    if(!GetConsoleMode(hConOut, &modeFlags)) { // If function fails, return value is ZERO
        return GetLastError();
    }

    modeFlags = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if(!SetConsoleMode(hConOut, modeFlags)) { // Enable VT processing (colours, cursor positioning, etc.)
        return GetLastError();
    }
    return 0;
}

int main() {
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

    printf(ESC "[5;10H");
    printf(ESC "[48;2;128;4;256m ");
    printf("test");
	
	getch();
    return 0;
}
