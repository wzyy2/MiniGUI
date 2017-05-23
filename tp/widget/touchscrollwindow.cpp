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
#include "touchscrollwindow.h"

#include "minigui/ctrl/scrollview.h"

#define TIMEVAL2MS(a) (a.tv_sec * 1000 + a.tv_usec / 1000)

// -----------------  WORKAROUND: MINIGUI internal struct ------------------
typedef struct _scrdata {
    unsigned short hScrollVal;
    unsigned short vScrollVal;
    unsigned short hScrollPageVal;
    unsigned short vScrollPageVal;

    /* scroll offset */
    int nContX;
    int nContY;
    int prevnContX;
    int prevnContY;
    /* content range */
    int nContWidth;
    int nContHeight;
    /* visible content range */
    unsigned short visibleWidth;
    unsigned short visibleHeight;

    /* content margins */
    unsigned short leftMargin;
    unsigned short topMargin;
    unsigned short rightMargin;
    unsigned short bottomMargin;

    HWND hSV;

    /* scrollbar mode */
    SBPolicyType sbPolicy;

    void (*move_content)(HWND, struct _scrdata*, BOOL);

} SCRDATA;

typedef struct _containerdata {
    /* scroll view window */
    HWND hSV;
    /* container flags, status ... */
    DWORD flags;

} CONTDATA;
typedef CONTDATA* PCONTDATA;

typedef struct _swdata {
    SCRDATA scrdata;

    /* handle of static container */
    HWND hContainer;
    /* private data of container */
    PCONTDATA pContdata;

    /* scroll view flags, status ... */
    DWORD flags;

} SWDATA;
typedef SWDATA* PSWDATA;
// -----------------  WORKAROUND: MINIGUI internal struct ------------------

TouchScrollWnd::TouchScrollWnd(HWND hWndParent, int x, int y, int w, int h, DWORD dwStyle,
    DWORD dwStylEx, int Id)
    : MGScrollWnd()
{
    PSWDATA pswdata;
    _pushed = FALSE;

    if (dwStyle & WS_VSCROLL) {
        _y_scroll = 1;
    }
    if (dwStyle & WS_HSCROLL) {
        _x_scroll = 1;
    }

    // scroll window will use WindowAdditionalData when create, we can't use default create function
    m_hWnd = CreateWindowEx(GetClass(), "", dwStyle | WS_CHILD, dwStylEx, Id, x,
        y, w, h, hWndParent, (DWORD)NULL);

    SetWindowAdditionalData(m_hWnd, (DWORD)this);

    pswdata = (PSWDATA)GetWindowAdditionalData2(m_hWnd);
    h_container = pswdata->hContainer;
    SetWindowAdditionalData(pswdata->hContainer, (DWORD)this);
    SetWindowCallbackProc(pswdata->hContainer, (WNDPROC)TouchScrollWnd::_WndProc);
}

TouchScrollWnd::~TouchScrollWnd()
{
    DestroyWindow();
}

void TouchScrollWnd::TouchMove(int x, int y)
{
    timeval tmp_time;
    long move_time;

    if (_pushed == TRUE) {
        int move_x = x - _down_x;
        int move_y = y - _down_y;

        gettimeofday(&tmp_time, NULL);
        move_time = TIMEVAL2MS(tmp_time);
        if (move_time - _down_time > 50) {
            if ((_x_scroll && (abs(move_x) > _x_trigger)) || ((abs(move_y) > _y_trigger) && _y_scroll)) {
                _down_time = move_time;
                _down_x = x;
                _down_y = y;
                _moved = TRUE;
                printf("flush %d %d \n", move_x, move_y);
                SetContenPos(GetContentX() + move_x * _scale * _x_scroll, GetContentY() + move_y * _scale * _y_scroll);
            }
        }
    }
}

BOOL TouchScrollWnd::WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret)
{
    timeval tmp_time;
    int x;
    int y;

    switch (iMsg) {
    case MSG_MOUSEMOVE:
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        if (_down_x == 0 && _down_y == 0) {
            _down_x = x;
            _down_y = y;
        }
        TouchMove(x, y);
        break;
    case MSG_LBUTTONDOWN:
        SetCapture(m_hWnd);
        _pushed = TRUE;
        _moved = FALSE;
        // x,y give by MSG_LBUTTONDOWN will be wrong in scroll window.
        _down_x = 0;
        _down_y = 0;
        gettimeofday(&tmp_time, NULL);
        _down_time = TIMEVAL2MS(tmp_time);
        break;
    case MSG_LBUTTONUP:
        if (_pushed) {
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            if (_down_x == 0 && _down_y == 0) {
                _down_x = x;
                _down_y = y;
            }
            TouchMove(x, y);
        }
        _pushed = FALSE;
        break;
    case MSG_NCLBUTTONDOWN:
        break;
    case MSG_NCLBUTTONUP:
        break;
    case MSG_HITTEST:
        *pret = HT_CLIENT;
        return TRUE;
    }
    return FALSE;
}