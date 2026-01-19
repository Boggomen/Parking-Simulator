// Game state management and main game loop functions

#ifndef GAME_HPP
#define GAME_HPP

#include <windows.h>
#include <gdiplus.h>
#include "car.hpp"

// Timer settings for game loop (~60 FPS)
#define GAME_TIMER_ID 1
#define GAME_TIMER_INTERVAL 16

// Game state variables (defined in game.cpp)
extern Car playerCar;
extern bool gameInitialized;
extern DWORD lastUpdateTime;

// Initialize game when entering SCREEN_GAME
void InitGame(HWND hwnd);

// Cleanup when leaving SCREEN_GAME
void CleanupGame(HWND hwnd);

// Update game logic (called from WM_TIMER)
void UpdateGame(float deltaTime);

// Draw the game screen
void DrawGameScreen(HDC hdc, RECT* clientRect);

// Check keyboard input state
void ProcessGameInput(bool& accelerate, bool& brake, bool& steerLeft, bool& steerRight, bool& handbrake);

// Handle key presses (like Escape to return to menu)
void HandleGameKeyDown(HWND hwnd, WPARAM wParam);

#endif
