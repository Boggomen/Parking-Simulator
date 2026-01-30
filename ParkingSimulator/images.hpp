#ifndef IMAGES_HPP
#define IMAGES_HPP

#include <windows.h>

// ============= Declaration of Functions =============
void LoadImages();
void FreeImages();
void DrawBitmap(HDC hdc, HBITMAP hBitmap, int x, int y, int width, int height);

// Returns the correct game background based on difficulty
HBITMAP GetGameBackground();

#endif