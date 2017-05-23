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

class ScrollExplorerButtonNotification : public MGCtrlNotification {
public:
    ScrollExplorer* _caller;
    ScrollExplorerButtonNotification(ScrollExplorer* caller) { _caller = caller; }

    virtual void OnCtrlNotified(MGWnd* sender, int id, int code, DWORD add_data)
    {
        if (_caller->isMoved())
            return;

        printf("isPushed \n");
    }
};

ScrollExplorer::ScrollExplorer(HWND hWndParent, int x, int y, int w, int h, DWORD dwStyle,
    DWORD dwStylEx, int Id)
    : TouchScrollWnd(hWndParent, x, y, w, h, dwStyle, dwStylEx, Id)
{
    button_notification = new ScrollExplorerButtonNotification(this);
    ShowPreview();
}

ScrollExplorer::~ScrollExplorer()
{
    for (auto iter = video_items.cbegin(); iter != video_items.cend(); iter++) {
        delete *iter;
    }

    delete button_notification;
}

void ScrollExplorer::ShowPreview()
{
    int item_height = GetHeight() / EXPLORER_ITEM_R;
    int item_width = GetWidth() / EXPLORER_ITEM_C;
    int i;

    /* fake data */
    for (i = 0; i < EXPLORER_ITEM_R; i++) {
        for (int j = 0; j < EXPLORER_ITEM_C; j++) {
            PreviewItem* tmp = new PreviewItem(&TPUtils::GetResource()->bmp_play, TPUtils::GetColorkey(),
                GetContainer(), item_width * j, item_height * i, item_width, item_height);

            video_items.push_back(tmp);
            video_items[i * EXPLORER_ITEM_C + j]->SetNotification(button_notification);
        }
    }

    for (i = EXPLORER_ITEM_R; i < EXPLORER_ITEM_R * 2; i++) {
        for (int j = 0; j < EXPLORER_ITEM_C; j++) {
            PreviewItem* tmp = new PreviewItem(&TPUtils::GetResource()->bmp_play, TPUtils::GetColorkey(),
                GetContainer(), item_width * j, item_height * i, item_width, item_height);

            video_items.push_back(tmp);
            video_items[i * EXPLORER_ITEM_C + j]->SetNotification(button_notification);
        }
    }

    SetContHeight(i * item_height);
}