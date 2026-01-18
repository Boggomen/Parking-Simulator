#include "windowHand.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "menu.hpp"
#include "options.hpp"

// Static member initialization
std::map<HWND, WindowHandler*> WindowHandler::g_windowMap;

WindowHandler::WindowHandler() : hwnd(nullptr) {}

WindowHandler::~WindowHandler() {
    if (hwnd) {
        g_windowMap.erase(hwnd);
        DestroyWindow(hwnd);
    }
}

bool WindowHandler::Create(HINSTANCE hInstance, const wchar_t* className, const wchar_t* windowName, int width, int height) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        className,
        windowName,
        WS_OVERLAPPED, // Changes to WS_OVERLAPPEDWINDOW if you want to resize window in the application
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd) {
        g_windowMap[hwnd] = this;
        return true;
    }
    return false;
}

HWND WindowHandler::GetHandle() const {
    return hwnd;
}

void WindowHandler::Show(int cmdShow) {
    if (hwnd) ShowWindow(hwnd, cmdShow);
}

void WindowHandler::Hide() {
    if (hwnd) ShowWindow(hwnd, SW_HIDE);
}

void WindowHandler::Invalidate() {
    if (hwnd) InvalidateRect(hwnd, nullptr, FALSE);
}

LRESULT CALLBACK WindowHandler::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto it = g_windowMap.find(hwnd);
    if (it != g_windowMap.end()) {
        return it->second->HandleMessage(msg, wParam, lParam);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT WindowHandler::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        
        // Draw based on current screen
        switch (currentScreen) {
            case SCREEN_MENU:
                DrawMainMenu(hdc, &clientRect);
                break;
            case SCREEN_OPTIONS:
                DrawOptionsScreen(hdc, &clientRect);
                break;
            case SCREEN_GAME:
                // Future: DrawGame(hdc, &clientRect);
                break;
        }
        
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        
        // Handle clicks based on current screen
        switch (currentScreen) {
            case SCREEN_MENU:
                HandleMenuClick(hwnd, x, y);
                break;
            case SCREEN_OPTIONS:
                if (HandleOptionsClick(hwnd, x, y)) {
                    currentScreen = SCREEN_MENU;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                break;
        }
        return 0;
    }
    case WM_DESTROY: {
        FreeImages();
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}