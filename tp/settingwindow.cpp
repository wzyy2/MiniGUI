/**
 * Copyright (C) 2017 Fuzhou Rockchip Electronics Co., Ltd
 * Author: Jacob Chen <jacob2.chen@rock-chips.com>
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "settingwindow.h"

#include "tpmainwindow.h"
#include "tputils.h"
#include "widget/touchmenu.h"

class SettingButtonNotification : public MGCtrlNotification {
public:
    SettingWnd* _caller;
    SettingButtonNotification(SettingWnd* caller)
    {
        _caller = caller;
    }

    virtual void OnCtrlNotified(MGWnd* sender, int id, int code, DWORD add_data)
    {
        if (sender == _caller->return_button) {
            if (code == STN_CLICKED) {
                TPMainWnd* main_window = (TPMainWnd*)_caller->WndFromHandle(_caller->GetParent());
                main_window->Return();
            }
        } else if (sender == _caller->system_button) {
            if (code == STN_CLICKED) {
                _caller->system_menu->ShowWindow(SW_SHOWNORMAL);
                _caller->video_menu->ShowWindow(SW_HIDE);
                _caller->system_button->SetState(RKButton::kEnumON);
                _caller->video_button->SetState(RKButton::kEnumOFF);
            }

        } else if (sender == _caller->video_button) {
            if (code == STN_CLICKED) {
                _caller->system_menu->ShowWindow(SW_HIDE);
                _caller->video_menu->ShowWindow(SW_SHOWNORMAL);
                _caller->system_button->SetState(RKButton::kEnumOFF);
                _caller->video_button->SetState(RKButton::kEnumON);
            }
        }
    }
};

class SettingtMenuNotification : public MGCtrlNotification {
public:
    SettingWnd* _caller;
    SettingtMenuNotification(SettingWnd* caller)
    {
        _caller = caller;
    }

    virtual void OnCtrlNotified(MGWnd* sender, int id, int code, DWORD add_data)
    {
        int index = add_data;

        if (sender == _caller->video_menu) {
            switch (_caller->video_menu->GetCurSel()) {
            case 0:

                break;
            case 1:

                break;
            }
        } else if (sender == _caller->system_menu) {
            switch (_caller->system_menu->GetCurSel()) {
            case 0:

                break;
            case 1:
                break;
            }
        }
    }
};

SettingWnd::SettingWnd(HWND hParent)
    : MGUserCtrl()
{
    RECT rc;

    MGUserCtrl::Register(GetClass(), 0);

    this->Create(hParent, 0, 0, g_rcScr.right, g_rcScr.bottom,
        WS_CHILD | WS_VISIBLE);

    button_notification = new SettingButtonNotification(this);
    menu_notification = new SettingtMenuNotification(this);

    return_button = new RKButton(m_hWnd, 0, 0, SIDE_W, RETURN_H);
    return_button->SetNotification(button_notification);
    return_button->SetTristate(&TPUtils::GetResource()->bmp_return);

    video_button = new RKButton(m_hWnd, 0, RETURN_H, SIDE_W, (GetHeight() - RETURN_H) / 2);
    video_button->SetTristate(&TPUtils::GetResource()->bmp_video_setting[0], &TPUtils::GetResource()->bmp_video_setting[1]);
    video_button->GetBound(&rc);
    video_button->SetNotification(button_notification);

    system_button = new RKButton(m_hWnd, 0, rc.bottom, SIDE_W, (GetHeight() - RETURN_H) / 2);
    system_button->SetTristate(&TPUtils::GetResource()->bmp_system_setting[0], &TPUtils::GetResource()->bmp_system_setting[1]);
    system_button->GetBound(&rc);
    system_button->SetNotification(button_notification);

    InitMenu();

    {
        button_notification->OnCtrlNotified(video_button, 0, STN_CLICKED, 0);
    }

    //background
    this->SetWindowBkColor(THEME_COLOR);
}

SettingWnd::~SettingWnd()
{
    delete button_notification;

    delete video_button;
    delete system_button;
    delete return_button;

    delete video_menu;
    delete system_menu;

    DestroyWindow();
}

void SettingWnd::InitMenu()
{
    // setting menu
    video_menu = new TouchMenu(m_hWnd, SIDE_W, 0,
        GetWidth() - SIDE_W, GetHeight(),
        WS_CHILD | WS_VSCROLL | WS_VISIBLE);
    video_menu->CreateLeft(m_hWnd, (GetWidth() + SIDE_W) / 2, 0,
        (GetWidth() - SIDE_W) / 2, GetHeight(),
        WS_CHILD | WS_VSCROLL | WS_VISIBLE);

    video_menu->SetDefaultItemHeight(SETTING_ITEM_H);
    video_menu->AddMenuItem("111\n");

    system_menu = new TouchMenu(m_hWnd, SIDE_W, 0,
        (GetWidth() - SIDE_W) / 2, GetHeight(),
        WS_CHILD | WS_VSCROLL | SVS_AUTOSORT);
    system_menu->CreateLeft(m_hWnd, (GetWidth() + SIDE_W) / 2, 0,
        (GetWidth() - SIDE_W) / 2, GetHeight(),
        WS_CHILD | WS_VSCROLL | SVS_AUTOSORT);

    system_menu->SetDefaultItemHeight(SETTING_ITEM_H);
    system_menu->AddMenuItem("record resulotion\n");
    system_menu->AddOption(0, "333\n");
    system_menu->AddOption(0, "444\n");

    system_menu->AddMenuItem("captrue resulotio\n");
    system_menu->AddMenuItem("record mode\n");

    system_menu->AddMenuItem("record time\n");
    system_menu->AddMenuItem("record time\n");
    system_menu->AddMenuItem("record time\n");
    system_menu->AddMenuItem("record time\n");
    system_menu->AddMenuItem("record time\n");
    system_menu->AddMenuItem("record time\n");
    system_menu->AddMenuItem("record time\n");
    system_menu->AddMenuItem("record time\n");
    system_menu->AddMenuItem("record time\n");
    system_menu->AddMenuItem("record time\n");

    video_menu->SetNotification(menu_notification);
    system_menu->SetNotification(menu_notification);
}

BOOL SettingWnd::WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret)
{

    return FALSE;
}