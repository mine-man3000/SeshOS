#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <tty.h>
#include <tty/fbCon.h>

bool escapeCompleted = false;
char escapeSequence[256];
int escapeSequenceLength = 0;
bool TTY_HandleEsc(char ch) {
    escapeSequence[escapeSequenceLength++] = ch;
    escapeSequence[escapeSequenceLength] = '\0';

    // Check if the escape sequence has completed
    if (ch >= 0x40 && ch <= 0x7E) {

        // Parse color codes from the escape sequence
        char* colorCodeStart = strstr(escapeSequence, "[");
        if (colorCodeStart != NULL) {
            char* colorCodeEnd = strstr(colorCodeStart, "m");
            if (colorCodeEnd != NULL) {
                // it was able to find the `m`, therefore it is now done
                escapeCompleted = true;
                // Extract the color code substring
                size_t codeLength = (size_t)(colorCodeEnd - colorCodeStart - 1);
                char colorCode[codeLength + 1];
                memcpy(colorCode, colorCodeStart + 1, codeLength);
                colorCode[codeLength] = '\0';

                // Handle color and attribute switching based on the color code
                int code = atoi(colorCode);
                switch (code) {
                    case 0:
                        // Reset all attributes
                        TTY_Color = COLOR_LGRAY;
                        TTY_BgColor = COLOR_BLACK;
                        TTY_Bold = false;
                        // TODO: TTY_Underline = false;
                        break;
                    case 1:
                        // Set text style to bold
                        TTY_Bold = true;
                        break;
                    case 22:
                        // Disable bold
                        TTY_Bold = false;
                        break;
                    case 30:
                        // Set text color to black
                        TTY_Color = COLOR_BLACK;
                        break;
                    case 31:
                        // Set text color to red
                        TTY_Color = COLOR_RED;
                        break;
                    case 32:
                        // Set text color to green
                        TTY_Color = COLOR_GREEN;
                        break;
                    case 33:
                        // Set text color to yellow
                        TTY_Color = COLOR_YELLOW;
                        break;
                    case 34:
                        // Set text color to blue
                        TTY_Color = COLOR_BLUE;
                        break;
                    case 35:
                        // Set text color to magenta
                        TTY_Color = COLOR_PURPLE;
                        break;
                    case 36:
                        // Set text color to cyan
                        TTY_Color = COLOR_CYAN;
                        break;
                    case 37:
                        // Set text color to white
                        TTY_Color = COLOR_LGRAY;
                        break;
                    case 40:
                        // Set background color to black
                        TTY_BgColor = COLOR_BLACK;
                        break;
                    case 41:
                        // Set background color to red
                        TTY_BgColor = COLOR_RED;
                        break;
                    case 42:
                        // Set background color to green
                        TTY_BgColor = COLOR_GREEN;
                        break;
                    case 43:
                        // Set background color to yellow
                        TTY_BgColor = COLOR_YELLOW;
                        break;
                    case 44:
                        // Set background color to blue
                        TTY_BgColor = COLOR_BLUE;
                        break;
                    case 45:
                        // Set background color to magenta
                        TTY_BgColor = COLOR_PURPLE;
                        break;
                    case 46:
                        // Set background color to cyan
                        TTY_BgColor = COLOR_CYAN;
                        break;
                    case 47:
                        // Set background color to white
                        TTY_BgColor = COLOR_LGRAY;
                        break;
                    default:
                        // Unsupported color code, do nothing
                        break;
                }
            }
        }
    }
    if (escapeCompleted) {
        escapeCompleted = false;
        escapeSequenceLength = 0;
        return true;
    }
    return false;    
}
