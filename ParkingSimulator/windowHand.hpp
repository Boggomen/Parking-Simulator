// Window handler class - creates and manages the game window

#ifndef WINDOWHAND_HPP
#define WINDOWHAND_HPP

#include <windows.h>
#include <map>

class WindowHandler {
public:
    WindowHandler();
    ~WindowHandler();

    // Create the window with given size and title
    bool Create(HINSTANCE hInstance, const wchar_t* className, const wchar_t* windowName, int width, int height);
    
    HWND GetHandle() const;
    void Show(int cmdShow = SW_SHOW);
    void Hide();
    void Invalidate();  // Request a redraw

private:
    HWND hwnd;

    // Static callback that Windows calls for messages
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // Handles messages for this window
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    // Maps window handles to WindowHandler objects
    static std::map<HWND, WindowHandler*> g_windowMap;
};

#endif