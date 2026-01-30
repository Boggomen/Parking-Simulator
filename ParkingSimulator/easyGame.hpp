#ifndef EASYGAME_HPP
#define EASYGAME_HPP

#include <windows.h>
#include <gdiplus.h>
#include "car.hpp"

// ============= Easy Game State Variables =============
extern Car easyPlayerCar;
extern bool easyGameInitialized;
extern DWORD easyLastUpdateTime;

// ============= Declaration of Functions =============
void InitEasyGame(HWND hwnd);
void CleanupEasyGame(HWND hwnd);
void UpdateEasyGame(float deltaTime);
void DrawEasyGameScreen(HDC hdc, RECT* clientRect);
void HandleEasyGameKeyDown(HWND hwnd, WPARAM wParam);

#endif
