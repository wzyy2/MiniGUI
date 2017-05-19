#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include "tpcommon.h"
#include "widget/rkbutton.h"

class PreviewWnd : public MGUserCtrl {
public:
    PreviewWnd(HWND hParent);
    ~PreviewWnd();
    DECLARE_CTRL_CLASS("preview")

private:
    BOOL WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret);
};

#endif
