// Global variables and constants shared across the project

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <windows.h>

// Screen states - which screen is currently shown
enum ScreenState { SCREEN_MENU, SCREEN_OPTIONS, SCREEN_GAME };
extern ScreenState currentScreen;

// Button IDs
#define BTN_START   1
#define BTN_OPTIONS 2
#define BTN_EXIT    3
#define BTN_BACK    4
#define BTN_RIGHT   5
#define BTN_LEFT    6

// Button position rectangles
extern RECT btnStartRect;
extern RECT btnOptionsRect;
extern RECT btnExitRect;
extern RECT btnBackRect;
extern RECT btnRightRect;
extern RECT btnLeftRect;

// Image handles - backgrounds
extern HBITMAP hBgImage;   // Main menu background
extern HBITMAP hOBgImage;  // Options background
extern HBITMAP hGBgImage;  // Game background

// Image handles - buttons
extern HBITMAP hBtnStart;
extern HBITMAP hBtnOptions;
extern HBITMAP hBtnExit;
extern HBITMAP hBtnBack;
extern HBITMAP hBtnRight;
extern HBITMAP hBtnLeft;

#endif