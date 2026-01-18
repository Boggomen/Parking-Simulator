#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <windows.h>
#include "globals.hpp"

// Free options screen images
void FreeOptionsImages();

// Draw the options screen
void DrawOptionsScreen(HDC hdc, RECT* clientRect);

// Handle clicks on the options screen
// Returns TRUE if we should go back to main menu
BOOL HandleOptionsClick(HWND hwnd, int x, int y);

#endif