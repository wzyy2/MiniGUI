#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "tpcommon.h"
#include "widget/rkbutton.h"
#include "widget/touchmenu.h"

class SettingButtonNotification;
class SettingtMenuNotification;

class SettingWnd : public MGUserCtrl {
    friend class SettingButtonNotification;
    friend class SettingtMenuNotification;

public:
    SettingWnd(HWND hParent);
    ~SettingWnd();
    DECLARE_CTRL_CLASS("setting")

private:

    SettingButtonNotification* button_notification;
    SettingtMenuNotification* menu_notification;

    RKButton* video_button;
    RKButton* system_button;
    RKButton* return_button;

    TouchMenu* video_menu;
    TouchMenu* system_menu;

    void InitMenu();
    BOOL WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret);
};

#endif
