#include <Windows.h>
#include <stdio.h>

#define ESC "\x1b"
#define cWidth 120
#define cHeight 30

char mainMenu[] =
"########################################################################################################################"
"########################################################################################################################"
"########################################################################################################################"
"########################################################################################################################"
"####/                   ####/                   ####/         #############         ####/                   ############"
"###/#                   ###/#                   ###/#    #    #############    #    ###/#                   ############"
"##|##                   ##|##                   ##|##    ##    ###########    ##    ##|##    ##############/############"
"##|##    ##############/##|##    ########|##    ##|##    \\##    #########    |##    ##|##    _____________/#############"
"##|##    _____________/###|##    ########|##    ##|##    #\\##    #######    #|##    ##|##    ###########################"
"##|##    #################|##    ########|##    ##|##    ##\\##    #####    ##|##    ##|##    ###########################"
"##|##    #################|##    ########|##    ##|##    ###\\##    ###    ###|##    ##|##    ###########################"
"##|##    #################|##    ########|##    ##|##    ####\\##    #    ####|##    ##|##                   ############"
"##|##    #######/       ##|##                   ##|##    #####\\##        ####|##    ##|##                   ############"
"##|##    ######/#       ##|##                   ##|##    ######\\/#######/####|##    ##|##    ##############/############"
"##|##    #####|#/###    ##|##    ########|##    ##|##    ######/_______/#####|##    ##|##    _____________/#############"
"##|##    #####|/____    ##|##    ########|##    ##|##    ####################|##    ##|##    ###########################"
"##|##    ###########    ##|##    ########|##    ##|##    ####################|##    ##|##    ###########################"
"##|##                   ##|##    ########|##    ##|##    ####################|##    ##|##    ###########################"
"##|##                   ##|##    ########|##    ##|##    ####################|##    ##|##                   ############"
"##|##                   ##|##    ########|##    ##|##    ####################|##    ##|##                   ############"
"##|#/##################/##|#/###/########|#/###/##|#/###/####################|#/###/##|#/##################/############"
"##|/__________________/###|/___/#########|/___/###|/___/#####################|/___/###|/__________________/#############"
"########################################################################################################################"
"########################################################################################################################"
"###############################################__##____###__####____####____############################################"
"##############################################//####||###//\\\\###||##\\####||#############################################"
"##############################################\\\\_###||##||__||##||__/####||#############################################"
"################################################\\\\##||##||__||##||##\\####||#############################################"
"###############################################_//##||##||##||##||###\\###||#############################################"
"########################################################################################################################"
"########################################################################################################################";

void DrawFrame(char *frame) {
    printf(ESC "7"); // Save cursor position in memory
    printf(ESC "[0;0H"); // Move cursor to origin on console
    /*for (int h = 0; h < cHeight-1; h++) {
        for (int w = 0; w < cWidth; w++) {
            printf("%s", frame); // screenBuff treated as a 1D array
        }
        printf(ESC "B" ESC "[%uD", cWidth); // Move down one line and "cWidth" amount left
    }*/
    printf("%s", frame); // screenBuff treated as a 1D array
    printf(ESC "8"); // Restore cursor position from memory
}

void AddFrameObject(wchar_t buff, int width, int height, int X, int Y) {
    // Take buffer of characters (buff), draw line by line starting at X, Y
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
    /*
    if(!GetConsoleMode(hConIn, &modeFlags)) { // If function fails, return value is ZERO
        return GetLastError();
    }

    modeFlags |= ENABLE_MOUSE_INPUT;
    if(!SetConsoleMode(hConIn, modeFlags)) { // Enable mouse input for console
        return GetLastError();
    }*/

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
    char *screenBuff = (char *) calloc(screenSize, sizeof(char)); // Allocate heap memory
    if (screenBuff == NULL) {
        printf("[-] Not enough memory; exiting.");
        return 1;
    }

    strcpy(screenBuff, mainMenu);
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

    // Run game loop here:

    /*
    printf(ESC "[5;10H");
    printf(ESC "[48;2;128;4;256m ");
    printf("test");
    */

    DrawFrame(screenBuff);
	
	getch();
    return 0;
}
