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
#include "rkbutton.h"

RKButton::RKButton(HWND hWndParent, int x, int y, int w, int h)
    : MGUserCtrl()
{
    _display_bmp = NULL;

    MGUserCtrl::Register(GetClass(), 0);

    this->Create(hWndParent, x, y, w, h,
        WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_REALSIZEIMAGE | SS_CENTERIMAGE | SS_NOTIFY);
}

RKButton::~RKButton()
{
    DestroyWindow();
}

int RKButton::SetState(enum State a)
{
    _state_back = _state;
    _state = a;

    switch (_state) {
    case kEnumOFF:
        if (_pbmp[0])
            SetBitmap(_pbmp[0]);
        break;
    case kEnumON:
        if (_pbmp[1])
            SetBitmap(_pbmp[1]);
        break;
    case kEnumSEL:
        if (_pbmp[2])
            SetBitmap(_pbmp[2]);
        break;
    }

    InvalidateRect();

    return 0;
}

BOOL RKButton::WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret)
{
    RECT rcClient;
    HDC hdc;
    const char* spCaption;
    UINT uFormat = DT_TOP;
    DWORD dwStyle;
    BOOL needShowCaption = TRUE;

    switch (iMsg) {
    case MSG_CREATE:
        *pret = 0;
        return TRUE;

    case STM_GETIMAGE:
        *pret = (int)_display_bmp;
        return TRUE;

    case STM_SETIMAGE: {
        int pOldValue;

        pOldValue = (int)_display_bmp;
        _display_bmp = (PBITMAP)wParam;
        ::InvalidateRect(m_hWnd, NULL,
            (::GetWindowStyle(m_hWnd) & SS_TYPEMASK) == SS_ICON);
        *pret = pOldValue;
        return TRUE;
    }

    case MSG_GETDLGCODE:
        *pret = DLGC_STATIC;
        return TRUE;

    case MSG_PAINT:
        hdc = ::BeginPaint(m_hWnd);
        ::SelectFont(hdc, ::GetWindowFont(m_hWnd));

        ::GetClientRect(m_hWnd, &rcClient);
        dwStyle = ::GetWindowStyle(m_hWnd);
        if (dwStyle & WS_DISABLED)
            SetTextColor(hdc, ::GetWindowElementPixelEx(m_hWnd, hdc, WE_FGC_DISABLED_ITEM));
        else
            SetTextColor(hdc, ::GetWindowElementPixelEx(m_hWnd, hdc, WE_FGC_WINDOW));

        // DK[11/01/2010]: For fix bug 4336
        switch (dwStyle & SS_TYPEMASK) {
        case SS_SIMPLE:
            spCaption = ::GetWindowCaption(m_hWnd);
            if (spCaption && !(dwStyle & SS_ALIGNMASK)) {
                SetBkMode(hdc, BM_TRANSPARENT);
                TextOut(hdc, 0, 0, spCaption);
                needShowCaption = FALSE;
            }
            break;

        case SS_GRAYRECT:
            SetBrushColor(hdc, PIXEL_lightgray);
            FillBox(hdc, 0, 0, rcClient.right, rcClient.bottom);
            break;

        case SS_BLACKRECT:
            SetBrushColor(hdc, PIXEL_black);
            FillBox(hdc, 0, 0, rcClient.right, rcClient.bottom);
            break;

        case SS_WHITERECT:
            SetBrushColor(hdc, PIXEL_lightwhite);
            FillBox(hdc, 0, 0, rcClient.right, rcClient.bottom);
            break;

        case SS_BLACKFRAME:
            SetPenColor(hdc, PIXEL_black);
            Rectangle(hdc, 0, 0, rcClient.right - 1, rcClient.bottom - 1);
            break;

        case SS_GRAYFRAME:
            SetPenColor(hdc, PIXEL_lightgray);
            Rectangle(hdc, 0, 0, rcClient.right - 1, rcClient.bottom - 1);
            break;

        case SS_WHITEFRAME:
            SetPenColor(hdc, PIXEL_lightwhite);
            Rectangle(hdc, 0, 0, rcClient.right - 1, rcClient.bottom - 1);
            break;

        case SS_BITMAP:
            if (_display_bmp) {
                int x = 0, y = 0, w, h;
                PBITMAP bmp = _display_bmp;

                if (dwStyle & SS_REALSIZEIMAGE) {
                    w = bmp->bmWidth;
                    h = bmp->bmHeight;
                    if (dwStyle & SS_CENTERIMAGE) {
                        x = (rcClient.right - w) >> 1;
                        y = (rcClient.bottom - h) >> 1;
                    }
                } else {
                    x = y = 0;
                    w = RECTW(rcClient);
                    h = RECTH(rcClient);
                }

                FillBoxWithBitmap(hdc, x, y, w, h, bmp);
                needShowCaption = FALSE;
            }
            break;

        default:
            break;
        }

        if (_state == kEnumSEL && !_pbmp[2]) {
            HDC mem_dc = CreateMemDC(RECTW(rcClient), RECTH(rcClient), 16, MEMDC_FLAG_HWSURFACE | MEMDC_FLAG_SRCALPHA,
                0x0000F000, 0x00000F00, 0x000000F0, 0x0000000F);
            SetBrushColor(mem_dc, RGBA2Pixel(mem_dc, 0xFF, 0xFF, 0xFF, 0x40));
            FillBox(mem_dc, 0, 0, RECTW(rcClient), RECTH(rcClient));
            BitBlt(mem_dc, 0, 0, RECTW(rcClient), RECTH(rcClient), hdc, 0, 0, 0);
            DeleteMemDC(mem_dc);
        }

        if (needShowCaption) {
            uFormat = DT_TOP;
            switch (dwStyle & SS_ALIGNMASK) {
            case SS_LEFT:
                uFormat |= DT_LEFT;
                break;
            case SS_CENTER:
                uFormat |= DT_CENTER;
                break;
            case SS_RIGHT:
                uFormat |= DT_RIGHT;
                break;
            }

            if (dwStyle & SS_LEFTNOWORDWRAP) {
                uFormat |= DT_SINGLELINE | DT_EXPANDTABS;
            } else {
                uFormat |= DT_WORDBREAK;
            }

            if (dwStyle & SS_NOPREFIX)
                uFormat |= DT_NOPREFIX;

            spCaption = ::GetWindowCaption(m_hWnd);

            if (spCaption) {
                SetBkMode(hdc, BM_TRANSPARENT);
                DrawText(hdc, spCaption, -1, &rcClient, uFormat);
            }
        }

        ::EndPaint(m_hWnd, hdc);
        *pret = 0;

        return TRUE;

    case MSG_NCLBUTTONDOWN:
        if (_state != kEnumSEL) {
            SetState(kEnumSEL);
        }
        break;

    case MSG_NCLBUTTONUP:
        if (_state == kEnumSEL) {
            SetState(_state_back);
            if (::GetWindowStyle(m_hWnd) & SS_NOTIFY)
                ::NotifyParent(m_hWnd, GetId(), STN_CLICKED);
        }
        break;

    case MSG_HITTEST:
        // HACK ： always use nc
        *pret = 0x12;
        return TRUE;

    case MSG_FONTCHANGED:
        InvalidateRect(NULL, TRUE);
        *pret = 0;
        return TRUE;

    case MSG_SETTEXT:
        ::SetWindowCaption(m_hWnd, (char*)lParam);
        ::InvalidateRect(m_hWnd, NULL, TRUE);
        *pret = 0;
        return TRUE;

    default:
        break;
    }

    return FALSE;
}