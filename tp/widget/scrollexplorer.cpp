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
#include "scrollexplorer.h"

#include "tputils.h"

ScrollExplorer::ScrollExplorer(HWND hWndParent, int x, int y, int w, int h, DWORD dwStyle,
    DWORD dwStylEx, int Id)
    : MGScrollView()
{

    Create(hWndParent, x, y, w, h, dwStyle, dwStylEx, Id);

    ShowPreview();
}

ScrollExplorer::~ScrollExplorer()
{
    DestroyWindow();
}

void ScrollExplorer::ShowPreview()
{
    int item_height = GetHeight() / EXPLORER_ITEM_R;
    int item_width = GetWidth() / EXPLORER_ITEM_C;
    int i;

    /* fake data */
    for (i = 0; i < EXPLORER_ITEM_R * 2; i++) {
        for (int j = 0; j < EXPLORER_ITEM_C; j++) {
            MGStatic tmp;

            video_items.push_back(tmp);

            // window created in here will be deleted in father window destory
            video_items[i * EXPLORER_ITEM_C + j].Create(m_hWnd, item_width * j, item_height * i,
                item_width, item_height, WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_REALSIZEIMAGE | SS_CENTERIMAGE);

            video_items[i * EXPLORER_ITEM_C + j].SetWindowBkColor(TPUtils::GetColorkey());
            video_items[i * EXPLORER_ITEM_C + j].SetBitmap(&TPUtils::GetResource()->bmp_play);
        }
    }

    SetContHeight(i * item_height);
}