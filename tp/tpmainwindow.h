#ifndef TPMAINWINDOW_H
#define TPMAINWINDOW_H

#include "tpcommon.h"
#include "widget/rkbutton.h"
#include "widget/videosurface.h"
#include "widget/loadingindicator.h"

class MainButtonNotification;

class TPMainWnd : public MGMainWnd {
    friend class MainButtonNotification;

public:
    static TPMainWnd* Instance();
    static void Destroy();

    void Return();

private:
    TPMainWnd();
    ~TPMainWnd();

    void InitButton();
    
    BOOL WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret);

    MainButtonNotification* button_notification;

    RKButton* preview_button;
    RKButton* sleep_button;
    RKButton* audio_button;
    RKButton* explorer_button;

    RKButton* setting_button;
    RKButton* captrue_button;
    RKButton* lock_button;
    RKButton* record_button;

    MGWnd* preview_window;
    MGWnd* explorer_window;
    MGWnd* setting_window;

    VideoSurface* front_video;
    VideoSurface* back_video;

    LoadingIndicator* loading_indicator;

    static TPMainWnd* pInstance;
};

#endif
