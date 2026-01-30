#ifndef GAME_HPP
#define GAME_HPP

#include <windows.h>
#include <gdiplus.h>
#include "car.hpp"

// ============= Game Timer Settings =============
#define GAME_TIMER_ID 1
#define GAME_TIMER_INTERVAL 16

// ============= Game State Variables =============
extern Car playerCar;
extern bool gameInitialized;
extern DWORD lastUpdateTime;

// ============= Declaration of Functions =============
void InitGame(HWND hwnd);
void CleanupGame(HWND hwnd);
void UpdateGame(float deltaTime);
void DrawGameScreen(HDC hdc, RECT* clientRect);
void ProcessGameInput(bool& accelerate, bool& brake, bool& steerLeft, bool& steerRight, bool& handbrake);
void HandleGameKeyDown(HWND hwnd, WPARAM wParam);

#endif
