// Main menu screen functions

#ifndef MENU_HPP
#define MENU_HPP

#include <windows.h>

void InitMenuButtons();                         // Set up menu buttons
void DrawMainMenu(HDC hdc, RECT* clientRect);   // Draw the main menu
void HandleMenuClick(HWND hwnd, int x, int y);  // Handle button clicks
BOOL PointInRect(int x, int y, RECT* r);        // Check if point is in rectangle

#endif