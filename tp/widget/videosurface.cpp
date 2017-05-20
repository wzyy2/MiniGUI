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
#include "videosurface.h"

VideoSurface::VideoSurface(int channel, int colorkey, HWND hWndParent, int x, int y, int w,
    int h, DWORD dwStyle, DWORD dwStylEx, int Id,
    int left)
    : MGUserCtrl()
    , _channel(channel)
{
    MGUserCtrl::Register(GetClass(), 0);

    this->Create(hWndParent, x, y, w, h, dwStyle, dwStylEx, Id);
    this->SetWindowBkColor(colorkey);
}

VideoSurface::~VideoSurface() { this->DestroyWindow(); }

BOOL VideoSurface::WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret)
{
    switch (iMsg) {
    case MSG_HITTEST:
        // HACK ： always use nc
        *pret = 0x12;
        return TRUE;
    }

    return FALSE;
}