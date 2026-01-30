#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <windows.h>

// ============= Screen States =============
enum ScreenState { SCREEN_MENU, SCREEN_OPTIONS, SCREEN_GAME };
extern ScreenState currentScreen;

// ============= Difficulty Levels =============
enum Difficulty { DIFF_EASY, DIFF_MEDIUM, DIFF_HARD, DIFF_CREATIVE };
extern Difficulty currentDifficulty;
extern Difficulty selectedDifficulty;

// ============= Button IDs =============
#define BTN_START   1
#define BTN_OPTIONS 2
#define BTN_EXIT    3
#define BTN_BACK    4
#define BTN_RIGHT   5
#define BTN_LEFT    6
#define BTN_OK      7

// ============= Button Rectangles =============
extern RECT btnStartRect;
extern RECT btnOptionsRect;
extern RECT btnExitRect;
extern RECT btnBackRect;
extern RECT btnRightRect;
extern RECT btnLeftRect;

// ============= Image Handles =============
// Background Images
extern HBITMAP hBgImage;
extern HBITMAP hOBgImage;
extern HBITMAP hGBgImage;

// Game Mode Backgrounds
extern HBITMAP hEasyBgImage;
extern HBITMAP hMediumBgImage;
extern HBITMAP hHardBgImage;
extern HBITMAP hCreativeBgImage;

// Button Images
extern HBITMAP hBtnStart;
extern HBITMAP hBtnOptions;
extern HBITMAP hBtnExit;
extern HBITMAP hBtnBack;
extern HBITMAP hBtnRight;
extern HBITMAP hBtnLeft;
extern HBITMAP hBtnOk;

// Difficulty Button Images
extern HBITMAP hBtnEasy;
extern HBITMAP hBtnMedium;
extern HBITMAP hBtnHard;
extern HBITMAP hBtnCreative;

#endif