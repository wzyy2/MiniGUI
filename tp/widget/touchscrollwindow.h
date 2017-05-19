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

    inline BOOL isPushed() {
        return _pushed;
    }

private:
    HWND h_container;
    BOOL _pushed;
    int _scale = 2;

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
