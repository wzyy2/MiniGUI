#ifndef TOUCHSCROLLWINDOW_H
#define TOUCHSCROLLWINDOW_H

#include "tpcommon.h"

class TouchScrollWnd : public MGScrollWnd {
public:
    TouchScrollWnd(HWND hWndParent, int x, int y, int w, int h, DWORD dwStyle,
        DWORD dwStylEx = 0, int Id = 0);
    ~TouchScrollWnd();

    inline HWND GetContainer() {
        return h_container;
    }

    inline void SetTouchSpeed(int speed) {
        _scale = speed;
    }

    inline void SetScrollDir(int scroll_x, int scroll_y) {
        _x_scroll = scroll_x;
        _y_scroll = scroll_y;
    }

    inline BOOL isPushed() {
        return _pushed;
    }

    inline BOOL isMoved() {
        return _moved;
    }

private:
    HWND h_container;

    BOOL _pushed;
    int _scale = -1;
    int _x_trigger = 20;
    int _y_trigger = 20;
    int _x_scroll = 0;
    int _y_scroll = 0;
    BOOL _moved = FALSE;
    int _down_x, _down_y;
    long _down_time;

    static int _WndProc(HWND hWnd, int iMsg, WPARAM wParam, LPARAM lParam)
    {
        TouchScrollWnd* pThis = (TouchScrollWnd*)GetWindowAdditionalData(hWnd);

        if (pThis) {
            int ret;
            if (pThis->WndProc(iMsg, wParam, lParam, &ret))
                return ret;
        }
        return DefaultContainerProc(hWnd, iMsg, wParam, lParam);
    }

    void TouchMove(int x, int y);

    BOOL WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret);
};

#endif
