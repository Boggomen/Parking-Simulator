#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <windows.h>

// ============= Declaration of Functions =============
void InitOptionsButtons();
void DrawOptionsScreen(HDC hdc, RECT* clientRect);
BOOL HandleOptionsClick(HWND hwnd, int x, int y);

// Cycles difficulty left or right
void CycleDifficulty(int direction);

// Returns the button image for the current selected difficulty
HBITMAP GetDifficultyButtonImage();

#endif