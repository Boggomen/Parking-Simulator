#ifndef IMAGES_HPP
#define IMAGES_HPP

#include <windows.h>

void LoadImages();
void FreeImages();
void DrawBitmap(HDC hdc, HBITMAP hBitmap, int x, int y, int width, int height);

#endif