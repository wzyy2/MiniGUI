#ifndef TPMAINWINDOW_H
#define TPMAINWINDOW_H

#include "tpcommon.h"
#include "widget/loadingindicator.h"
#include "widget/rkbutton.h"
#include "widget/videosurface.h"

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

    MainButtonNotification* button_notification = NULL;

    RKButton* preview_button = NULL;
    RKButton* sleep_button = NULL;
    RKButton* audio_button = NULL;
    RKButton* explorer_button = NULL;

    RKButton* setting_button = NULL;
    RKButton* captrue_button = NULL;
    RKButton* lock_button = NULL;
    RKButton* record_button = NULL;

    MGWnd* preview_window = NULL;
    MGWnd* explorer_window = NULL;
    MGWnd* setting_window = NULL;

    VideoSurface* front_video = NULL;
    VideoSurface* back_video = NULL;

    LoadingIndicator* loading_indicator = NULL;

    static TPMainWnd* pInstance;
};

#endif
