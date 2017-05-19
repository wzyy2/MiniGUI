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
#include "explorerwindow.h"

#include "tpmainwindow.h"
#include "tputils.h"

class ExplorerButtonNotification : public MGCtrlNotification {
public:
    ExplorerWnd* _caller;
    ExplorerButtonNotification(ExplorerWnd* caller) { _caller = caller; }
    void ChangeActiveLeft(RKButton* sender)
    {
        if (_caller->active_left)
            _caller->active_left->SetState(RKButton::kEnumOFF);
        _caller->active_left = sender;
        _caller->active_left->SetState(RKButton::kEnumON);
    }

    void ChangeActiveTop(RKButton* sender)
    {
        if (_caller->active_top)
            _caller->active_top->SetState(RKButton::kEnumOFF);
        _caller->active_top = sender;
        _caller->active_top->SetState(RKButton::kEnumON);
    }

    void FlushPreview(RKButton* sender)
    {
        if (_caller->active_top != NULL && _caller->active_left != NULL) {
            delete _caller->scroll_explorer;
            _caller->scroll_explorer = new ScrollExplorer(
                _caller->m_hWnd, SIDE_W, RETURN_H, _caller->GetWidth() - SIDE_W,
                _caller->GetHeight() - RETURN_H, WS_CHILD | WS_VISIBLE | WS_VSCROLL);
            _caller->scroll_explorer->SetWindowBkColor(THEME_COLOR);
        }
    }

    virtual void OnCtrlNotified(MGWnd* sender, int id, int code, DWORD add_data)
    {
        if (sender == _caller->return_button) {
            if (code == STN_CLICKED) {
                TPMainWnd* main_window = (TPMainWnd*)_caller->WndFromHandle(_caller->GetParent());
                main_window->Return();
            }
        } else if (sender == _caller->video_button || sender == _caller->lock_button || sender == _caller->photo_button) {
            if (code == STN_CLICKED) {
                if (_caller->active_left != sender) {
                    ChangeActiveLeft((RKButton*)sender);
                    FlushPreview((RKButton*)sender);
                }
            }
        } else if (sender == _caller->front_button || sender == _caller->back_button) {
            if (code == STN_CLICKED) {
                if (_caller->active_top != sender) {
                    ChangeActiveTop((RKButton*)sender);
                    FlushPreview((RKButton*)sender);
                }
            }
        }
    }
};

ExplorerWnd::ExplorerWnd(HWND hParent)
    : MGUserCtrl()
{
    active_left = NULL;
    active_top = NULL;
    scroll_explorer = NULL;

    MGUserCtrl::Register(GetClass(), 0);

    this->Create(hParent, 0, 0, g_rcScr.right, g_rcScr.bottom,
        WS_CHILD | WS_VISIBLE);

    button_notification = new ExplorerButtonNotification(this);
    InitButton();

    //background
    this->SetWindowBkColor(THEME_COLOR);
}

ExplorerWnd::~ExplorerWnd()
{
    delete return_button;

    delete video_button;
    delete photo_button;
    delete lock_button;

    delete front_button;
    delete back_button;

    delete button_notification;
    delete scroll_explorer;

    DestroyWindow();
}

void ExplorerWnd::InitButton()
{
    RECT rc;

    return_button = new RKButton(m_hWnd, 0, 0, SIDE_W, RETURN_H);
    return_button->GetBound(&rc);
    return_button->SetNotification(button_notification);
    return_button->SetTristate(&TPUtils::GetResource()->bmp_return);

    video_button = new RKButton(m_hWnd, 0, rc.bottom, SIDE_W, (GetHeight() - RETURN_H) / 3);
    video_button->SetTristate(&TPUtils::GetResource()->bmp_video_explorer[0],
        &TPUtils::GetResource()->bmp_video_explorer[1]);
    video_button->GetBound(&rc);
    video_button->SetNotification(button_notification);

    photo_button = new RKButton(m_hWnd, 0, rc.bottom, SIDE_W, (GetHeight() - RETURN_H) / 3);
    photo_button->SetWindowBkColor(THEME_COLOR);
    photo_button->SetTristate(&TPUtils::GetResource()->bmp_photo_explorer[0],
        &TPUtils::GetResource()->bmp_photo_explorer[1]);
    photo_button->GetBound(&rc);
    photo_button->SetNotification(button_notification);

    lock_button = new RKButton(m_hWnd, 0, rc.bottom, SIDE_W, (GetHeight() - RETURN_H) / 3);
    lock_button->SetTristate(&TPUtils::GetResource()->bmp_lock_explorer[0],
        &TPUtils::GetResource()->bmp_lock_explorer[1]);
    lock_button->GetBound(&rc);
    lock_button->SetNotification(button_notification);

    front_button = new RKButton(m_hWnd, rc.right, 0, (GetWidth() - SIDE_W) / 2, RETURN_H);
    front_button->SetTristate(&TPUtils::GetResource()->bmp_front[0],
        &TPUtils::GetResource()->bmp_front[1]);
    front_button->GetBound(&rc);
    front_button->SetNotification(button_notification);

    back_button = new RKButton(m_hWnd, rc.right, 0, (GetWidth() - SIDE_W) / 2, RETURN_H);
    back_button->SetTristate(&TPUtils::GetResource()->bmp_back[0],
        &TPUtils::GetResource()->bmp_back[1]);
    back_button->GetBound(&rc);
    back_button->SetNotification(button_notification);

    {
        button_notification->OnCtrlNotified(front_button, 0, STN_CLICKED, 0);
        button_notification->OnCtrlNotified(video_button, 0, STN_CLICKED, 0);
    }
}

BOOL ExplorerWnd::WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret)
{
    return FALSE;
}