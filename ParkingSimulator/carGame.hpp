#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <windows.h>

void InitGameButtons();
void DrawGameScreen(HDC hdc, RECT* clientRect);
BOOL HandleGameClick(HWND hwnd, int x, int y);

#endif