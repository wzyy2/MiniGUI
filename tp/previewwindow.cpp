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
#include "previewwindow.h"

#include "tpmainwindow.h"

PreviewWnd::PreviewWnd(HWND hParent)
    : MGUserCtrl()
{
    MGUserCtrl::Register(GetClass(), 0);

    this->Create(hParent, 0, 0, g_rcScr.right, g_rcScr.bottom,
        WS_CHILD | WS_VISIBLE);

    //background
    this->SetWindowBkColor(THEME_COLOR);
}

PreviewWnd::~PreviewWnd()
{
    DestroyWindow();
}

BOOL PreviewWnd::WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret)
{

    return FALSE;
}