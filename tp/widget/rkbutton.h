#ifndef RKBUTTON_H
#define RKBUTTON_H

#include "tpcommon.h"

#define SS_VCENTER 0x00000040L

class RKButton : public MGUserCtrl {
public:
    RKButton(HWND hWndParent, int x, int y, int w, int h,
        DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_REALSIZEIMAGE | SS_CENTERIMAGE | SS_NOTIFY);
    ~RKButton();
    DECLARE_CTRL_CLASS("rkbutton")

    enum State {
        kEnumOFF,
        kEnumON,
        kEnumSEL,
    };

    inline void SetTristate(PBITMAP off, PBITMAP on = NULL, PBITMAP sel = NULL)
    {
        _pbmp[0] = off;
        _pbmp[1] = on;
        _pbmp[2] = sel;

        SetState(kEnumOFF);
    }

    inline void SetTextColor2(gal_pixel color)
    {
        text_color = color;
        InvalidateRect();
    }

    inline PBITMAP SetBitmap(PBITMAP pbmp)
    {
        return (PBITMAP)SendMessage(STM_SETIMAGE, (WPARAM)pbmp);
    }

    inline enum State GetState()
    {
        return _state;
    }

    int SetState(enum State a);

private:
    enum State _state = kEnumOFF;
    enum State _state_back = kEnumOFF;

    gal_pixel text_color = COLOR_lightwhite;

    PBITMAP _pbmp[3];

    PBITMAP _display_bmp = NULL;

    BOOL WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret);
};

#endif
