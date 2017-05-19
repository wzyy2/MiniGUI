#ifndef LOADINGINDICATOR_H
#define LOADINGINDICATOR_H

#include "tpcommon.h"

class LoadingIndicator {
public:
    LoadingIndicator(ANIMATION* anim, HWND hWndParent, int x, int y, int w, int h)
    {
        m_hWnd = ::CreateWindow(CTRL_ANIMATION, "", WS_VISIBLE | ANS_AUTOLOOP, 190,
            x, y, w, h, hWndParent, (DWORD)anim);
    }
    ~LoadingIndicator()
    {
        ::DestroyWindow(m_hWnd);
    }

    inline int StartPlay() { return ::SendMessage(m_hWnd, ANM_STARTPLAY, 0, 0); }

protected:
    HWND m_hWnd;
};

#endif