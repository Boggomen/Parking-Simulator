#ifndef MENU_HPP
#define MENU_HPP

#include <windows.h>

void InitMenuButtons();  // Add this
void DrawMainMenu(HDC hdc, RECT* clientRect);
void HandleMenuClick(HWND hwnd, int x, int y);
BOOL PointInRect(int x, int y, RECT* r);

#endif