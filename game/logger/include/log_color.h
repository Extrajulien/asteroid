#pragma once
#include <ctype.h>
#include <stdio.h>

static const char COLORCODE_TOKEN = '$';

typedef enum LogColor {
    COLOR_INVALID,
    COLOR_RESET,
    COLOR_RED,
    COLOR_YELLOW,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_PURPLE,
    COLOR_CYAN,
    COLOR_ORANGE,
    COLOR_GRAY,
    COLOR_BRIGHT_RED,
    COLOR_BRIGHT_YELLOW,
    COLOR_BRIGHT_GREEN,
    COLOR_BRIGHT_BLUE,
    COLOR_BRIGHT_PURPLE,
    COLOR_BRIGHT_CYAN,
    COLOR_BRIGHT_ORANGE,
    COLOR_BRIGHT_GRAY
} LogColor;

static LogColor GetColorFromChar(const char c) {
    const char lowerCase = tolower(c);
    switch (lowerCase) {
        case 'x': return COLOR_RESET;
        case 'r': return c == lowerCase ? COLOR_RED : COLOR_BRIGHT_RED;
        case 'y': return c == lowerCase ? COLOR_YELLOW : COLOR_BRIGHT_YELLOW;
        case 'g': return c == lowerCase ? COLOR_GREEN : COLOR_BRIGHT_GREEN;
        case 'b': return c == lowerCase ? COLOR_BLUE : COLOR_BRIGHT_BLUE;
        case 'p': return c == lowerCase ? COLOR_PURPLE : COLOR_BRIGHT_PURPLE;
        case 'c': return c == lowerCase ? COLOR_CYAN : COLOR_BRIGHT_CYAN;
        case 'o': return c == lowerCase ? COLOR_ORANGE : COLOR_BRIGHT_ORANGE;
        case 'd': return c == lowerCase ? COLOR_GRAY : COLOR_BRIGHT_GRAY;
        default: return COLOR_INVALID;
    }
}

static void ChangeColor(const LogColor color) {
    switch (color) {
        case COLOR_INVALID:        putchar(COLORCODE_TOKEN); break;
        case COLOR_RESET:          fputs("\x1b[0m", stdout); break;

        case COLOR_RED:            fputs("\x1b[31m", stdout); break;
        case COLOR_YELLOW:         fputs("\x1b[33m", stdout); break;
        case COLOR_GREEN:          fputs("\x1b[32m", stdout); break;
        case COLOR_BLUE:           fputs("\x1b[34m", stdout); break;
        case COLOR_PURPLE:         fputs("\x1b[35m", stdout); break;
        case COLOR_CYAN:           fputs("\x1b[36m", stdout); break;
        case COLOR_GRAY:           fputs("\x1b[38;5;238m", stdout); break;
        case COLOR_ORANGE:         fputs("\x1b[38;5;166m", stdout); break;
        case COLOR_BRIGHT_RED:     fputs("\x1b[91m", stdout); break;
        case COLOR_BRIGHT_YELLOW:  fputs("\x1b[93m", stdout); break;
        case COLOR_BRIGHT_GREEN:   fputs("\x1b[92m", stdout); break;
        case COLOR_BRIGHT_BLUE:    fputs("\x1b[94m", stdout); break;
        case COLOR_BRIGHT_PURPLE:  fputs("\x1b[38;5;141m", stdout); break;
        case COLOR_BRIGHT_CYAN:    fputs("\x1b[96m", stdout); break;
        case COLOR_BRIGHT_ORANGE:  fputs("\x1b[38;5;208m", stdout); break;
        case COLOR_BRIGHT_GRAY:    fputs("\x1b[38;5;246m", stdout); break;
    }
}