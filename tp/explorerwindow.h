#ifndef EXPLORERWINDOW_H
#define EXPLORERWINDOW_H

#include "scrollexplorer.h"
#include "tpcommon.h"
#include "widget/rkbutton.h"

class ExplorerButtonNotification;

class ExplorerWnd : public MGUserCtrl {
    friend class ExplorerButtonNotification;

public:
    ExplorerWnd(HWND hParent);
    ~ExplorerWnd();
    DECLARE_CTRL_CLASS("explorer")

private:
    void InitButton();

    RKButton* return_button;

    RKButton* video_button;
    RKButton* photo_button;
    RKButton* lock_button;

    RKButton* front_button;
    RKButton* back_button;

    RKButton* active_left;
    RKButton* active_top;

    ScrollExplorer* scroll_explorer;

    ExplorerButtonNotification* button_notification;

    BOOL WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret);
};

#endif
