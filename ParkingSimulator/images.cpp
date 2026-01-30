#include "images.hpp"
#include "globals.hpp"

// ============= Background Images =============
HBITMAP hBgImage = NULL;
HBITMAP hOBgImage = NULL;  
HBITMAP hGBgImage = NULL;

// ============= Game Mode Backgrounds =============
HBITMAP hEasyBgImage = NULL;
HBITMAP hMediumBgImage = NULL;
HBITMAP hHardBgImage = NULL;
HBITMAP hCreativeBgImage = NULL;

// ============= Button Images =============
HBITMAP hBtnStart = NULL;
HBITMAP hBtnOptions = NULL;
HBITMAP hBtnExit = NULL;
HBITMAP hBtnBack = NULL;       
HBITMAP hBtnRight = NULL;      
HBITMAP hBtnLeft = NULL;
HBITMAP hBtnOk = NULL;

// ============= Difficulty Button Images =============
HBITMAP hBtnEasy = NULL;
HBITMAP hBtnMedium = NULL;
HBITMAP hBtnHard = NULL;
HBITMAP hBtnCreative = NULL;

// ============= Load Images Function =============
void LoadImages() {
    // Main backgrounds
    hBgImage = (HBITMAP)LoadImage(NULL, L"resources/mainmenu_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hOBgImage = (HBITMAP)LoadImage(NULL, L"resources/options_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hGBgImage = (HBITMAP)LoadImage(NULL, L"resources/game_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // Game mode backgrounds
    hEasyBgImage = (HBITMAP)LoadImage(NULL, L"resources/easyGame_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hMediumBgImage = (HBITMAP)LoadImage(NULL, L"resources/mediumGame_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hHardBgImage = (HBITMAP)LoadImage(NULL, L"resources/hardGame_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hCreativeBgImage = (HBITMAP)LoadImage(NULL, L"resources/creativeGame_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // Menu buttons
    hBtnStart = (HBITMAP)LoadImage(NULL, L"resources/btn_start.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBtnOptions = (HBITMAP)LoadImage(NULL, L"resources/btn_options.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBtnExit = (HBITMAP)LoadImage(NULL, L"resources/btn_exit.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // Options buttons
    hBtnBack = (HBITMAP)LoadImage(NULL, L"resources/btn_back.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBtnRight = (HBITMAP)LoadImage(NULL, L"resources/right_arrow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBtnLeft = (HBITMAP)LoadImage(NULL, L"resources/left_arrow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBtnOk = (HBITMAP)LoadImage(NULL, L"resources/btn_ok.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // Difficulty buttons
    hBtnEasy = (HBITMAP)LoadImage(NULL, L"resources/btn_easy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBtnMedium = (HBITMAP)LoadImage(NULL, L"resources/btn_medium.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBtnHard = (HBITMAP)LoadImage(NULL, L"resources/btn_hard.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBtnCreative = (HBITMAP)LoadImage(NULL, L"resources/btn_creative.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

// ============= Free Images Function =============
void FreeImages() {
    // Background Images
    if (hBgImage) DeleteObject(hBgImage);
    if (hOBgImage) DeleteObject(hOBgImage);
    if (hGBgImage) DeleteObject(hGBgImage);

    // Game mode backgrounds
    if (hEasyBgImage) DeleteObject(hEasyBgImage);
    if (hMediumBgImage) DeleteObject(hMediumBgImage);
    if (hHardBgImage) DeleteObject(hHardBgImage);
    if (hCreativeBgImage) DeleteObject(hCreativeBgImage);

    // Button Images
    if (hBtnStart) DeleteObject(hBtnStart);
    if (hBtnOptions) DeleteObject(hBtnOptions);
    if (hBtnExit) DeleteObject(hBtnExit);
    if (hBtnBack) DeleteObject(hBtnBack);
    if (hBtnRight) DeleteObject(hBtnRight);
    if (hBtnLeft) DeleteObject(hBtnLeft);
    if (hBtnOk) DeleteObject(hBtnOk);

    // Difficulty buttons
    if (hBtnEasy) DeleteObject(hBtnEasy);
    if (hBtnMedium) DeleteObject(hBtnMedium);
    if (hBtnHard) DeleteObject(hBtnHard);
    if (hBtnCreative) DeleteObject(hBtnCreative);
}

// ============= Draw Bitmap Function =============
void DrawBitmap(HDC hdc, HBITMAP hBitmap, int x, int y, int width, int height) {
    if (hBitmap == NULL) return;

    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBitmap);
    BitBlt(hdc, x, y, width, height, hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);
}

// ============= Get Game Background Function =============
HBITMAP GetGameBackground() {
    switch (currentDifficulty) {
        case DIFF_EASY:
            return hEasyBgImage ? hEasyBgImage : hGBgImage;
        case DIFF_MEDIUM:
            return hMediumBgImage ? hMediumBgImage : hGBgImage;
        case DIFF_HARD:
            return hHardBgImage ? hHardBgImage : hGBgImage;
        case DIFF_CREATIVE:
            return hCreativeBgImage ? hCreativeBgImage : hGBgImage;
        default:
            return hGBgImage;
    }
}