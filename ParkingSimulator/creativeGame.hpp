#ifndef CREATIVEGAME_HPP
#define CREATIVEGAME_HPP

#include <windows.h>
#include <gdiplus.h>
#include "car.hpp"

// ============= Easy Game State Variables =============
extern Car creativePlayerCar;
extern bool creativeGameInitialized;
extern DWORD creativeLastUpdateTime;

// ============= Declaration of Functions =============
void InitCreativeGame(HWND hwnd);
void CleanupCreativeGame(HWND hwnd);
void UpdateCreativeGame(float deltaTime);
void DrawEasyCreativeScreen(HDC hdc, RECT* clientRect);
void HandleCreativeGameKeyDown(HWND hwnd, WPARAM wParam);

#endif
