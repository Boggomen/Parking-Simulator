#include "creativeGame.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "game.hpp"
#include "borders.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace Gdiplus;

// ============= Creative Game State Variables =============
Car creativePlayerCar;
bool creativeGameInitialized = false;
DWORD creativeLastUpdateTime = 0;
int hitCount = 0;
Chicken chickens[MAX_CHICKENS];

static HWND creativeHwnd = NULL;
static Borders creativeBorders;

// ============= Chicken Images (GDI+) =============
static Image* chickenAliveImg = nullptr;
static Image* chickenDeadImg = nullptr;
static Image* chickenLegImg = nullptr;

// ============= Car Evolution Phase =============
static int currentCarPhase = 0;

// ============= Helper: Get car phase from hitCount =============
static int GetCarPhase(int hits) {
    if (hits >= 15) return 3;
    if (hits >= 8) return 2;
    if (hits >= 3) return 1;
    return 0;
}

// ============= Helper: Get car image path for phase =============
static const wchar_t* GetCarImageForPhase(int phase) {
    switch (phase) {
        case 1:  return L"resources/car_bloodPhase1.png";
        case 2:  return L"resources/car_bloodPhase2.png";
        case 3:  return L"resources/car_bloodPhase3.png";
        default: return L"resources/car.png";
    }
}

// ============= Helper: Spawn chicken at random position =============
static void SpawnChicken(Chicken* chicken, RECT* clientRect) {
    float chickenW = 30.0f;
    float chickenH = 30.0f;
    if (chickenAliveImg) {
        chickenW = (float)chickenAliveImg->GetWidth();
        chickenH = (float)chickenAliveImg->GetHeight();
    }

    chicken->width = chickenW;
    chicken->height = chickenH;

    float margin = 50.0f;
    float minX = margin;
    float maxX = (float)clientRect->right - margin;
    float minY = margin;
    float maxY = (float)clientRect->bottom - margin;

    if (maxX <= minX) maxX = minX + 100.0f;
    if (maxY <= minY) maxY = minY + 100.0f;

    chicken->x = minX + (float)(rand() % (int)(maxX - minX));
    chicken->y = minY + (float)(rand() % (int)(maxY - minY));
    chicken->alive = true;
    chicken->active = true;
    chicken->deathTime = 0;
}

// ============= Helper: Check collision =============
static bool CheckCollision(float carX, float carY, float carW, float carH,
                           float chickenX, float chickenY, float chickenW, float chickenH) {
    float carRadius = sqrtf((carW / 2) * (carW / 2) + (carH / 2) * (carH / 2)) * 0.6f;
    float chickenRadius = (chickenW + chickenH) * 0.25f;

    float dx = carX - chickenX;
    float dy = carY - chickenY;
    float dist = sqrtf(dx * dx + dy * dy);

    return dist < (carRadius + chickenRadius);
}

// ============= Helper: Load GDI+ image safely =============
static Image* LoadGdiplusImage(const wchar_t* path) {
    Image* img = Image::FromFile(path);
    if (img && img->GetLastStatus() == Ok) {
        return img;
    }
    if (img) { delete img; }
    return nullptr;
}

// ============= Initialize Function =============
void InitCreativeGame(HWND hwnd) {
    if (creativeGameInitialized) return;

    srand((unsigned int)GetTickCount());
    creativeHwnd = hwnd;

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float startX = (float)(clientRect.right / 2);
    float startY = (float)(clientRect.bottom / 2);

    creativePlayerCar.Init(startX, startY, 0.0f);

    if (!creativePlayerCar.LoadCarImage(L"resources/car.png")) {
        OutputDebugString(L"Warning: Could not load car.png, using fallback rendering\n");
    }

    InitBorders(&creativeBorders, &clientRect, creativePlayerCar.width, creativePlayerCar.height);

    hitCount = 0;
    currentCarPhase = 0;

    chickenAliveImg = LoadGdiplusImage(L"resources/chickenAlive.png");
    chickenDeadImg = LoadGdiplusImage(L"resources/chickenDead.png");
    chickenLegImg = LoadGdiplusImage(L"resources/chickenLeg.png");

    for (int i = 0; i < MAX_CHICKENS; i++) {
        SpawnChicken(&chickens[i], &clientRect);
    }

    SetTimer(hwnd, GAME_TIMER_ID, GAME_TIMER_INTERVAL, NULL);
    creativeLastUpdateTime = (DWORD)GetTickCount64();
    lastUpdateTime = GetTickCount();

    creativeGameInitialized = true;
}

// ============= Cleanup Function =============
void CleanupCreativeGame(HWND hwnd) {
    if (!creativeGameInitialized) return;

    KillTimer(hwnd, GAME_TIMER_ID);

    if (chickenAliveImg) { delete chickenAliveImg; chickenAliveImg = nullptr; }
    if (chickenDeadImg)  { delete chickenDeadImg;  chickenDeadImg = nullptr; }
    if (chickenLegImg)   { delete chickenLegImg;   chickenLegImg = nullptr; }

    creativeGameInitialized = false;
}

// ============= Input Function =============
void ProcessCreativeGameInput(bool& accelerate, bool& brake, bool& steerLeft, bool& steerRight, bool& handbrake) {
    accelerate = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000);
    brake = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000);
    steerLeft = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000);
    steerRight = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000);
    handbrake = (GetAsyncKeyState(VK_SPACE) & 0x8000);
}

// ============= Update Function =============
void UpdateCreativeGame(float deltaTime) {
    if (!creativeGameInitialized) return;

    bool accelerate, brake, steerLeft, steerRight, handbrake;
    ProcessCreativeGameInput(accelerate, brake, steerLeft, steerRight, handbrake);
    creativePlayerCar.Update(deltaTime, accelerate, brake, steerLeft, steerRight, handbrake);

    ApplyBorderCollision(&creativeBorders, &creativePlayerCar.x, &creativePlayerCar.y,
                         &creativePlayerCar.velocityX, &creativePlayerCar.velocityY);

    DWORD now = (DWORD)GetTickCount64();
    RECT clientRect;
    GetClientRect(creativeHwnd, &clientRect);

    for (int i = 0; i < MAX_CHICKENS; i++) {
        if (!chickens[i].active) {
            SpawnChicken(&chickens[i], &clientRect);
            continue;
        }

        if (chickens[i].alive) {
            if (creativePlayerCar.speed > 10.0f &&
                CheckCollision(creativePlayerCar.x, creativePlayerCar.y,
                               creativePlayerCar.width, creativePlayerCar.height,
                               chickens[i].x, chickens[i].y,
                               chickens[i].width, chickens[i].height)) {
                chickens[i].alive = false;
                chickens[i].deathTime = now;
                hitCount++;

                int newPhase = GetCarPhase(hitCount);
                if (newPhase != currentCarPhase) {
                    currentCarPhase = newPhase;
                    creativePlayerCar.LoadCarImage(GetCarImageForPhase(currentCarPhase));
                }
            }
        } else {
            if (now - chickens[i].deathTime >= CHICKEN_DEATH_DELAY) {
                chickens[i].active = false;
            }
        }
    }
}

// ============= Display Function =============
void DrawEasyCreativeScreen(HDC hdc, RECT* clientRect) {
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, clientRect->right, clientRect->bottom);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

    HBITMAP bgImage = GetGameBackground();
    if (bgImage) {
        HDC hdcBg = CreateCompatibleDC(hdcMem);
        SelectObject(hdcBg, bgImage);

        BITMAP bm;
        GetObject(bgImage, sizeof(BITMAP), &bm);

        StretchBlt(hdcMem, 0, 0, clientRect->right, clientRect->bottom,
            hdcBg, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        DeleteDC(hdcBg);
    }
    else {
        HBRUSH brush = CreateSolidBrush(RGB(60, 60, 60));
        FillRect(hdcMem, clientRect, brush);
        DeleteObject(brush);
    }

    Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

    // Draw chickens
    for (int i = 0; i < MAX_CHICKENS; i++) {
        if (!chickens[i].active) continue;

        Image* chickenImg = chickens[i].alive ? chickenAliveImg : chickenDeadImg;
        if (chickenImg) {
            float halfW = chickens[i].width / 2.0f;
            float halfH = chickens[i].height / 2.0f;
            graphics.DrawImage(chickenImg,
                chickens[i].x - halfW, chickens[i].y - halfH,
                chickens[i].width, chickens[i].height);
        } else {
            SolidBrush chickenBrush(chickens[i].alive ?
                Color(255, 255, 200, 0) : Color(255, 150, 50, 50));
            graphics.FillEllipse(&chickenBrush,
                chickens[i].x - 15.0f, chickens[i].y - 15.0f, 30.0f, 30.0f);
        }
    }

    // Draw car
    creativePlayerCar.Draw(&graphics);

    // Draw speed display
    wchar_t speedText[64];
    swprintf_s(speedText, L"km/h: %.0f", (creativePlayerCar.speed / 10));

    Font font(L"Arial", 14);
    SolidBrush textBrush(Color(255, 255, 255, 255));

    graphics.DrawString(speedText, -1, &font, PointF(15.0f, 12.0f), &textBrush);

    // Draw "Creative Mode" header
    graphics.DrawString(L"Creative Mode", -1, &font, PointF(15.0f, 42.0f), &textBrush);

    // Draw hit counter with chickenLeg icon
    float counterY = 70.0f;
    float iconSize = 22.0f;

    float iconX = 15.0f;
    if (chickenLegImg) {
        graphics.DrawImage(chickenLegImg, iconX, counterY + 1.0f, iconSize, iconSize);
        iconX += iconSize + 2.0f;
    }

    wchar_t hitText[32];
    swprintf_s(hitText, L": %d", hitCount);
    graphics.DrawString(hitText, -1, &font, PointF(iconX, counterY + 2.0f), &textBrush);

    // Draw controls at bottom
    Font smallFont(L"Arial", 10);
    SolidBrush hintBrush(Color(200, 200, 200, 200));
    graphics.DrawString(L"WASD/Arrows: Drive | Space: Handbrake | Esc: Menu", -1, &smallFont,
        PointF(10.0f, (float)(clientRect->bottom - 25)), &hintBrush);

    BitBlt(hdc, 0, 0, clientRect->right, clientRect->bottom, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);
}

// ============= Key Handler =============
void HandleCreativeGameKeyDown(HWND hwnd, WPARAM wParam) {
    if (wParam == VK_ESCAPE) {
        CleanupCreativeGame(hwnd);
        currentScreen = SCREEN_MENU;
        InvalidateRect(hwnd, NULL, TRUE);
    }
}
