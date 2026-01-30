#ifndef WINDOWHAND_HPP
#define WINDOWHAND_HPP

#include <windows.h>
#include <map>

// ============= Window Handler Class =============
class WindowHandler {
public:
    WindowHandler();
    ~WindowHandler();

    bool Create(HINSTANCE hInstance, const wchar_t* className, const wchar_t* windowName, int width, int height);
    HWND GetHandle() const;
    void Show(int cmdShow = SW_SHOW);
    void Hide();
    void Invalidate();

private:
    HWND hwnd;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    static std::map<HWND, WindowHandler*> g_windowMap;
};

#endif