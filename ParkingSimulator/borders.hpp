#ifndef BORDERS_HPP
#define BORDERS_HPP

#include <windows.h>
#include <algorithm>

// ============= Border Structure =============
struct Borders {
    float left;
    float top;
    float right;
    float bottom;
    float carWidth;
    float carHeight;
};

// ============= Inline Helper Functions =============

// Initialize borders based on window size and car size
inline void InitBorders(Borders* borders, RECT* clientRect, float carW, float carH) {
    // Half dimensions for collision (car position is at center)
    float halfW = carW / 2.0f;
    float halfH = carH / 2.0f;
    
    // Use the larger dimension for diagonal car rotation
    float carRadius = (halfW > halfH) ? halfW : halfH;
    
    borders->left = carRadius;
    borders->top = carRadius;
    borders->right = (float)clientRect->right - carRadius;
    borders->bottom = (float)clientRect->bottom - carRadius;
    borders->carWidth = carW;
    borders->carHeight = carH;
}

// Check and apply collision with borders
inline void ApplyBorderCollision(Borders* borders, float* x, float* y, float* velX, float* velY) {
    // Left border
    if (*x < borders->left) {
        *x = borders->left;
        *velX = 0;
    }
    
    // Right border
    if (*x > borders->right) {
        *x = borders->right;
        *velX = 0;
    }
    
    // Top border
    if (*y < borders->top) {
        *y = borders->top;
        *velY = 0;
    }
    
    // Bottom border
    if (*y > borders->bottom) {
        *y = borders->bottom;
        *velY = 0;
    }
}

#endif
