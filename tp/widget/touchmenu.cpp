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
#include "touchmenu.h"
#include "rkbutton.h"

class MenuNotification : public MGCtrlNotification {
public:
    TouchMenu* _caller;
    MenuNotification(TouchMenu* caller)
    {
        _caller = caller;
    }

    virtual void OnCtrlNotified(MGWnd* sender, int id, int code, DWORD add_data)
    {
        if (_caller->isMoved())
            return;

        if (code == STN_CLICKED) {
            delete _caller->left_menu;
            _caller->left_menu = NULL;
            _caller->left_menu = new TouchMenu(_caller->GetParent(), _caller->_x + _caller->_w,
                _caller->_y, _caller->_w, _caller->_h, _caller->_dwStyle, 0, 0, TRUE);
            _caller->left_menu->SetDefaultHeight(_caller->GetDefaultHeight());
            _caller->options_create[sender](_caller->left_menu, sender);
        }
    }
};

TouchMenu::TouchMenu(HWND hWndParent, int x, int y, int w, int h,
    DWORD dwStyle, DWORD dwStylEx, int Id, int left)
    : TouchScrollWnd(hWndParent, x, y, w, h, dwStyle, dwStylEx, Id)
    , _x(x)
    , _y(y)
    , _w(w)
    , _h(h)
    , _dwStyle(dwStyle)
{
    _left = left;

    if (!_left) {
        menu_notification = new MenuNotification(this);
    }
}

TouchMenu::~TouchMenu()
{
    for (auto iter = settings.cbegin(); iter != settings.cend(); iter++) {
        delete *iter;
    }

    for (auto iter = left_options.cbegin(); iter != left_options.cend(); iter++) {
        delete *iter;
    }

    delete menu_notification;
    delete left_menu;
}

BOOL TouchMenu::AddSetting(const char* desc, OPTIONFUNC callback)
{
    if (_left)
        return FALSE;

    RKButton* tmp;
    tmp = new RKButton(GetContainer(), 0, settings.size() * _default_height, GetWidth(), _default_height,
        WS_CHILD | WS_VISIBLE | SS_VCENTER | SS_NOTIFY);
    tmp->SetWindowText(desc);
    tmp->SetNotification(menu_notification);
    tmp->SetWindowBkColor(GetWindowBkColor());

    settings.push_back(tmp);
    SetContHeight(settings.size() * _default_height);
    options_create[tmp] = callback;

    return TRUE;
}
