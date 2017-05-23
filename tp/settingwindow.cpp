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
                delete _caller->video_menu;
                _caller->video_menu = NULL;
                _caller->CreateSystemMenu();
                _caller->system_button->SetState(RKButton::kEnumON);
                _caller->video_button->SetState(RKButton::kEnumOFF);
            }

        } else if (sender == _caller->video_button) {
            if (code == STN_CLICKED) {
                delete _caller->system_menu;
                _caller->system_menu = NULL;
                _caller->CreateVideoMenu();
                _caller->system_button->SetState(RKButton::kEnumOFF);
                _caller->video_button->SetState(RKButton::kEnumON);
            }
        }
    }
};

SettingWnd::SettingWnd(HWND hParent)
    : MGUserCtrl()
    , video_menu(NULL)
    , system_menu(NULL)
{
    RECT rc;

    MGUserCtrl::Register(GetClass(), 0);

    this->Create(hParent, 0, 0, g_rcScr.right, g_rcScr.bottom,
        WS_CHILD | WS_VISIBLE);

    button_notification = new SettingButtonNotification(this);

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

    button_notification->OnCtrlNotified(video_button, 0, STN_CLICKED, 0);

    //background
    this->SetWindowBkColor(COLOR_lightwhite);
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

BOOL SettingWnd::WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret)
{

    return FALSE;
}

MGButton* SettingWnd::AddRadioButton(TouchMenu* parent, const char* desc)
{
    MGButton* tmp;
    DWORD dwStyle = WS_CHILD | BS_AUTORADIOBUTTON | WS_VISIBLE;
    if (parent->GetOptionSize() == 0)
        dwStyle |= WS_GROUP;
    tmp = new MGButton();
    tmp->Create(parent->GetHandle(), 0, parent->GetOptionSize() * parent->GetDefaultHeight(),
        parent->GetWidth(), parent->GetDefaultHeight(), dwStyle);
    tmp->SetWindowText(desc);
    tmp->SetWindowBkColor(COLOR_lightwhite);

    parent->AddOtion(tmp);

    return tmp;
}

void SettingWnd::CreateSystemMenu()
{
    system_menu = new TouchMenu(m_hWnd, SIDE_W, 0,
        (GetWidth() - SIDE_W) / 2, GetHeight(),
        WS_CHILD | WS_VISIBLE | WS_VSCROLL);

    system_menu->SetWindowBkColor(THEME_COLOR);
    system_menu->AddSetting("Boot mode", SettingWnd::CreateSystemOption);
    system_menu->AddSetting("Date", SettingWnd::CreateSystemOption);
    system_menu->AddSetting("Gravity", SettingWnd::CreateSystemOption);
    system_menu->AddSetting("Parking guard", SettingWnd::CreateSystemOption);
    system_menu->AddSetting("Language", SettingWnd::CreateSystemOption);
    system_menu->AddSetting("Format", SettingWnd::CreateSystemOption);
}

void SettingWnd::CreateVideoMenu()
{
    video_menu = new TouchMenu(m_hWnd, SIDE_W, 0,
        (GetWidth() - SIDE_W) / 2, GetHeight(),
        WS_CHILD | WS_VISIBLE | WS_VSCROLL);

    video_menu->SetWindowBkColor(THEME_COLOR);
    video_menu->AddSetting("Video resolution", SettingWnd::CreateVideoOption);
    video_menu->AddSetting("Capture resolution", SettingWnd::CreateVideoOption);
    video_menu->AddSetting("Video mode", SettingWnd::CreateVideoOption);
    video_menu->AddSetting("Video duration", SettingWnd::CreateVideoOption);
    video_menu->AddSetting("Exposure", SettingWnd::CreateVideoOption);
    video_menu->AddSetting("Record", SettingWnd::CreateVideoOption);
    video_menu->AddSetting("Watermark", SettingWnd::CreateVideoOption);
}

void SettingWnd::CreateSystemOption(TouchMenu* parent, MGWnd* sender)
{
    SettingWnd* wnd = (SettingWnd*)WndFromHandle(parent->GetParent());
    char* text = new char[sender->GetWindowTextLength() + 2];

    sender->GetWindowText(text, sender->GetWindowTextLength() + 2);
    if (!strcmp(text, "Video resolution")) {
    }

    delete text;
}

void SettingWnd::CreateVideoOption(TouchMenu* parent, MGWnd* sender)
{
    SettingWnd* wnd = (SettingWnd*)WndFromHandle(parent->GetParent());
    char* text = new char[sender->GetWindowTextLength() + 2];

    sender->GetWindowText(text, sender->GetWindowTextLength() + 2);
    if (!strcmp(text, "Video resolution")) {
        MGButton* tmp;
        char option[2][20] = { "1080p", "720p" };

        auto func
            = [](HWND hWnd, int id, int code, DWORD add_data) {
                  printf("Hello world");
              };

        tmp = wnd->AddRadioButton(parent, option[0]);
        ::SetNotificationCallback(tmp->GetHandle(), (NOTIFPROC)func);
        tmp = wnd->AddRadioButton(parent, option[1]);
        ::SetNotificationCallback(tmp->GetHandle(), (NOTIFPROC)func);
        tmp = wnd->AddRadioButton(parent, option[1]);
        ::SetNotificationCallback(tmp->GetHandle(), (NOTIFPROC)func);
    }

    delete text;
}