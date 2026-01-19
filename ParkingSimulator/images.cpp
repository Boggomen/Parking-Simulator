#include "images.hpp"
#include "globals.hpp"

// Background Images
HBITMAP hBgImage = NULL;
HBITMAP hOBgImage = NULL;  
HBITMAP hGBgImage = NULL;

//Car Sprite
HBITMAP raceCar = NULL;

// Button Images
HBITMAP hBtnStart = NULL;
HBITMAP hBtnOptions = NULL;
HBITMAP hBtnExit = NULL;
HBITMAP hBtnBack = NULL;       
HBITMAP hBtnRight = NULL;      
HBITMAP hBtnLeft = NULL;       

// Putting the images into varibles
void LoadImages() {
    hBgImage = (HBITMAP)LoadImage(
        NULL,
        L"resources/mainmenu_bg.bmp", // Main Menu Background
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hOBgImage = (HBITMAP)LoadImage(
        NULL,
        L"resources/options_bg.bmp", // Options Background
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hGBgImage = (HBITMAP)LoadImage(
        NULL,
        L"resources/game_bg.bmp", // Game Background
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    raceCar = (HBITMAP)LoadImage(
        NULL,
        L"car.bmp", // Car Sprite
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnStart = (HBITMAP)LoadImage(
        NULL,
        L"resources/btn_start.bmp", // Start Button
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnOptions = (HBITMAP)LoadImage(
        NULL,
        L"resources/btn_options.bmp", // Options Button
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnExit = (HBITMAP)LoadImage(
        NULL,
        L"resources/btn_exit.bmp", // Exit Button
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnBack = (HBITMAP)LoadImage( 
        NULL,
        L"resources/btn_back.bmp", // Back Button
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnRight = (HBITMAP)LoadImage(
        NULL,
        L"resources/right_arrow.bmp", // Right Arrow Button
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnLeft = (HBITMAP)LoadImage(
        NULL,
        L"resources/left_arrow.bmp", // Left Arrow Button
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );
}

void FreeImages() {
    // Background Images
    if (hBgImage) DeleteObject(hBgImage);
    if (hOBgImage) DeleteObject(hOBgImage);
    if (hGBgImage) DeleteObject(hGBgImage);

    // Car Sprite
    if (raceCar) DeleteObject(raceCar);

    // Button Images
    if (hBtnStart) DeleteObject(hBtnStart);
    if (hBtnOptions) DeleteObject(hBtnOptions);
    if (hBtnExit) DeleteObject(hBtnExit);
    if (hBtnBack) DeleteObject(hBtnBack);
    if (hBtnRight) DeleteObject(hBtnRight);
    if (hBtnLeft) DeleteObject(hBtnLeft);
}

void DrawBitmap(HDC hdc, HBITMAP hBitmap, int x, int y, int width, int height) {
    if (hBitmap == NULL) return;

    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBitmap);
    BitBlt(hdc, x, y, width, height, hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);
}