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
#include "tpmainwindow.h"

#include "explorerwindow.h"
#include "previewwindow.h"
#include "settingwindow.h"
#include "tputils.h"

TPMainWnd* TPMainWnd::pInstance = NULL;

class MainButtonNotification : public MGCtrlNotification {
public:
    TPMainWnd* _caller;
    MainButtonNotification(TPMainWnd* caller)
    {
        _caller = caller;
    }

    virtual void OnCtrlNotified(MGWnd* sender, int id, int code, DWORD add_data)
    {
        printf("OnCtrlNotified \n");
        if (code == STN_CLICKED) {
            if (sender == _caller->preview_button) {
                if (_caller->preview_window)
                    delete _caller->preview_window;
                _caller->preview_window = new PreviewWnd(_caller->m_hWnd);
            } else if (sender == _caller->explorer_button) {
                if (_caller->explorer_window)
                    delete _caller->explorer_window;
                _caller->explorer_window = new ExplorerWnd(_caller->m_hWnd);
            } else if (sender == _caller->setting_button) {
                if (_caller->setting_window)
                    delete _caller->setting_window;
                _caller->setting_window = new SettingWnd(_caller->m_hWnd);
            } else if (sender == _caller->audio_button) {
                if (_caller->audio_button->GetState() == RKButton::kEnumOFF) {
                    _caller->audio_button->SetState(RKButton::kEnumON);
                } else {
                    _caller->audio_button->SetState(RKButton::kEnumOFF);
                }
            } else if (sender == _caller->lock_button) {
                if (_caller->lock_button->GetState() == RKButton::kEnumOFF) {
                    _caller->lock_button->SetState(RKButton::kEnumON);
                } else {
                    _caller->lock_button->SetState(RKButton::kEnumOFF);
                }
            } else if (sender == _caller->record_button) {
                if (_caller->record_button->GetState() == RKButton::kEnumOFF) {
                    _caller->record_button->SetState(RKButton::kEnumON);
                } else {
                    _caller->record_button->SetState(RKButton::kEnumOFF);
                }
            } else if (sender == _caller->captrue_button) {
                if (_caller->loading_indicator == NULL) {
                    _caller->loading_indicator = new LoadingIndicator(TPUtils::GetResource()->anim_wait,
                        _caller->m_hWnd, _caller->GetWidth() / 2 - 98, _caller->GetHeight() / 2 - 98, 98, 98);
                    _caller->loading_indicator->StartPlay();
                }
                else {
                    delete _caller->loading_indicator;
                    _caller->loading_indicator = NULL;
                }
            } else if (sender == _caller->sleep_button) {
                TPUtils::Suspend();
            }
        }
    }
};

TPMainWnd::TPMainWnd()
{
    BOOL ret;

    preview_window = NULL;
    explorer_window = NULL;
    setting_window = NULL;

    ret = MGMainWnd::Create(WS_VISIBLE | WS_HIDEMENUBAR | WS_WITHOUTCLOSEMENU,
        "rearview",
        0, 0, g_rcScr.right, g_rcScr.bottom,
        HWND_DESKTOP,
        0, 0, 0, COLOR_lightwhite,
        WS_EX_NONE | WS_EX_AUTOSECONDARYDC,
        parameter_get_video_lan());
    if (ret == FALSE)
        throw std::runtime_error("TPMainWnd init failed");

    this->SetAlpha(MEMDC_FLAG_SWSURFACE, 0);

    InitButton();

    // videosurface
    TPUtils::SetColorkey(GetWindowElementPixel(m_hWnd, WE_BGC_DESKTOP));
    front_video = new VideoSurface(0, m_hWnd, MAIN_BTN_W * 2, 0,
        GetWidth() / 2 - MAIN_BTN_W, GetHeight(),
        WS_CHILD | WS_VISIBLE);
    back_video = new VideoSurface(1, m_hWnd, GetWidth() / 2 + MAIN_BTN_W, 0,
        GetWidth() / 2 - MAIN_BTN_W, GetHeight(),
        WS_CHILD | WS_VISIBLE);

    // subwindow
    // preview_window = new PreviewWnd(m_hWnd);
    //explorer_window = new ExplorerWnd(m_hWnd);
    // setting_window = new SettingWnd(m_hWnd);

    //background
    this->SetWindowBkColor(THEME_COLOR);
}

TPMainWnd::~TPMainWnd()
{
    delete preview_button;
    delete sleep_button;
    delete audio_button;
    delete explorer_button;

    delete setting_button;
    delete captrue_button;
    delete lock_button;
    delete record_button;

    delete button_notification;
    delete loading_indicator;
}

TPMainWnd* TPMainWnd::Instance()
{
    if (pInstance == NULL) {
        pInstance = new TPMainWnd();
    }
    return pInstance;
}

void TPMainWnd::Destroy()
{
    delete pInstance;
    pInstance = NULL;
}

void TPMainWnd::Return()
{
    if (preview_window) {
        delete preview_window;
        preview_window = NULL;
    }
    if (explorer_window) {
        delete explorer_window;
        explorer_window = NULL;
    }
    if (setting_window) {
        delete setting_window;
        setting_window = NULL;
    }
}

void TPMainWnd::InitButton()
{
    RECT rc;

    button_notification = new MainButtonNotification(this);

    preview_button = new RKButton(m_hWnd, 0, 0, MAIN_BTN_W, GetHeight() / 4);
    preview_button->GetBound(&rc);
    preview_button->SetTristate(&TPUtils::GetResource()->bmp_preview, NULL);

    sleep_button = new RKButton(m_hWnd, 0, rc.bottom, MAIN_BTN_W, GetHeight() / 4);
    sleep_button->GetBound(&rc);
    sleep_button->SetTristate(&TPUtils::GetResource()->bmp_sleep, NULL);

    audio_button = new RKButton(m_hWnd, 0, rc.bottom, MAIN_BTN_W, GetHeight() / 4);
    audio_button->GetBound(&rc);
    audio_button->SetTristate(&TPUtils::GetResource()->bmp_audio[0], &TPUtils::GetResource()->bmp_audio[1]);

    explorer_button = new RKButton(m_hWnd, 0, rc.bottom, MAIN_BTN_W, GetHeight() / 4);
    explorer_button->GetBound(&rc);
    explorer_button->SetTristate(&TPUtils::GetResource()->bmp_explorer, NULL);

    setting_button = new RKButton(m_hWnd, MAIN_BTN_W, 0, MAIN_BTN_W, GetHeight() / 4);
    setting_button->GetBound(&rc);
    setting_button->SetTristate(&TPUtils::GetResource()->bmp_setting, NULL);

    captrue_button = new RKButton(m_hWnd, MAIN_BTN_W, rc.bottom, MAIN_BTN_W, GetHeight() / 4);
    captrue_button->GetBound(&rc);
    captrue_button->SetTristate(&TPUtils::GetResource()->bmp_captrue, NULL);

    lock_button = new RKButton(m_hWnd, MAIN_BTN_W, rc.bottom, MAIN_BTN_W, GetHeight() / 4);
    lock_button->GetBound(&rc);
    lock_button->SetTristate(&TPUtils::GetResource()->bmp_lock[0], &TPUtils::GetResource()->bmp_lock[1]);

    record_button = new RKButton(m_hWnd, MAIN_BTN_W, rc.bottom, MAIN_BTN_W, GetHeight() / 4);
    record_button->GetBound(&rc);
    record_button->SetTristate(&TPUtils::GetResource()->bmp_record[0], &TPUtils::GetResource()->bmp_record[1]);

    preview_button->SetNotification(button_notification);
    audio_button->SetNotification(button_notification);
    sleep_button->SetNotification(button_notification);
    captrue_button->SetNotification(button_notification);
    lock_button->SetNotification(button_notification);
    record_button->SetNotification(button_notification);
    explorer_button->SetNotification(button_notification);
    setting_button->SetNotification(button_notification);
}

BOOL TPMainWnd::WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret)
{
    switch (iMsg) {
    case MSG_CREATE:
        break;
    }

    return FALSE;
}