#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "tpcommon.h"
#include "widget/rkbutton.h"
#include "widget/touchmenu.h"

class SettingButtonNotification;

class SettingWnd : public MGUserCtrl {
    friend class SettingButtonNotification;

public:
    SettingWnd(HWND hParent);
    ~SettingWnd();
    DECLARE_CTRL_CLASS("setting")

private:
    SettingButtonNotification* button_notification;

    RKButton* video_button;
    RKButton* system_button;
    RKButton* return_button;

    TouchMenu* video_menu;
    TouchMenu* system_menu;

    BOOL WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret);

    MGButton* AddRadioButton(TouchMenu* parent, const char* desc);

    void CreateSystemMenu();
    void CreateVideoMenu();

    static void CreateSystemOption(TouchMenu* parent, MGWnd* sender);
    static void CreateVideoOption(TouchMenu* parent, MGWnd* sender);
};

#endif
