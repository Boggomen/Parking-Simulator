#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <windows.h>

// Screen states
enum ScreenState { SCREEN_MENU, SCREEN_OPTIONS, SCREEN_GAME };
extern ScreenState currentScreen;

// Button IDs
#define BTN_START   1
#define BTN_OPTIONS 2
#define BTN_EXIT    3
#define BTN_BACK    4
#define BTN_RIGHT   5
#define BTN_LEFT    6

// Button positions (x, y, width, height)
extern RECT btnStartRect;
extern RECT btnOptionsRect;
extern RECT btnExitRect;
extern RECT btnBackRect;
extern RECT btnRightRect;
extern RECT btnLeftRect;

// Image handles
extern HBITMAP hBgImage;
extern HBITMAP hOBgImage;
extern HBITMAP hBtnStart;
extern HBITMAP hBtnOptions;
extern HBITMAP hBtnExit;
extern HBITMAP hBtnBack;
extern HBITMAP hBtnRight;
extern HBITMAP hBtnLeft;

#endif