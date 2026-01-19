// Image loading and management functions

#ifndef IMAGES_HPP
#define IMAGES_HPP

#include <windows.h>

// Load all game images from resources folder
void LoadImages();

// Free all loaded images (call on exit)
void FreeImages();

// Draw a bitmap at specified position and size
void DrawBitmap(HDC hdc, HBITMAP hBitmap, int x, int y, int width, int height);

#endif