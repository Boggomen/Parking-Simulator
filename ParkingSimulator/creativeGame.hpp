#ifndef CREATIVEGAME_HPP
#define CREATIVEGAME_HPP

#include <windows.h>
#include <gdiplus.h>
#include "car.hpp"

// ============= Chicken Constants =============
#define MAX_CHICKENS 6
#define CHICKEN_DEATH_DELAY 1000

// ============= Chicken Structure =============
struct Chicken {
    float x, y;
    float width, height;
    bool alive;
    bool active;
    DWORD deathTime;
};

// ============= Creative Game State Variables =============
extern Car creativePlayerCar;
extern bool creativeGameInitialized;
extern DWORD creativeLastUpdateTime;
extern int hitCount;
extern Chicken chickens[MAX_CHICKENS];

// ============= Declaration of Functions =============
void InitCreativeGame(HWND hwnd);
void CleanupCreativeGame(HWND hwnd);
void UpdateCreativeGame(float deltaTime);
void DrawEasyCreativeScreen(HDC hdc, RECT* clientRect);
void HandleCreativeGameKeyDown(HWND hwnd, WPARAM wParam);

#endif
