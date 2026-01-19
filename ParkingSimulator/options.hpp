#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <windows.h>

void InitOptionsButtons();
void DrawOptionsScreen(HDC hdc, RECT* clientRect);
BOOL HandleOptionsClick(HWND hwnd, int x, int y);

#endif