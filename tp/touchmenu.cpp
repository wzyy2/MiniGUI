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

TouchMenu::TouchMenu(HWND hWndParent, int x, int y, int w, int h,
    DWORD dwStyle, DWORD dwStylEx, int Id, int left)
    : MGScrollView()
{
    item_height = 100;

    Create(hWndParent, x, y, w, h, dwStyle, dwStylEx, Id);

    if (!left) {
        SetItemDraw((SVITEM_DRAWFUNC)Draw);
    }
}

TouchMenu::~TouchMenu()
{
    DestroyWindow();
    delete left_menu;
}

void TouchMenu::CreateLeft(HWND hWndParent, int x, int y, int w, int h,
    DWORD dwStyle, DWORD dwStylEx, int Id)
{
    left_menu = new TouchMenu(hWndParent, x, y, w, h, dwStyle, dwStylEx, Id, TRUE);
    left_menu->SetItemDraw((SVITEM_DRAWFUNC)DrawLeft);
}

BOOL TouchMenu::AddMenuItem(const char* desc)
{
    SVITEMINFO item;
    HSVITEM item_h;
    struct MenuElement element;

    item.nItem = save_items.size();
    item.nItemHeight = item_height;
    item.addData = (DWORD)desc;

    if (AddItem(&item, &item_h))
        return FALSE;

    element.info = item;
    element.handle = item_h;

    save_items.push_back(element);

    return TRUE;
}

BOOL TouchMenu::AddOption(int index, const char* desc)
{
    save_items.at(index).options.push_back(desc);

    return TRUE;
}

void TouchMenu::FlushOption(int index)
{
    SVITEMINFO item;
    int i;

    if (!left_menu || index < 0)
        return;

    // delete all options
    for (i = 0; i < left_menu->save_items.size(); i++) {
        left_menu->DeleteItem(left_menu->save_items[i].info.nItem,
            left_menu->save_items[i].handle);
    }
    left_menu->save_items.clear();

    // add new options
    for (i = 0; i < save_items.at(index).options.size(); i++) {
        left_menu->AddMenuItem(save_items.at(index).options[i]);
    }
}

void TouchMenu::Draw(HWND hWnd, GHANDLE hsvi, HDC hdc, RECT* rcDraw)
{
    MGScrollView* pThis = (MGScrollView*)GetWindowAdditionalData(hWnd);
    const char* label = (const char*)scrollview_get_item_adddata(hsvi);

    SetBkMode(hdc, BM_TRANSPARENT);
    SetTextColor(hdc, PIXEL_black);

    if (scrollview_is_item_hilight(hWnd, hsvi)) {
        SetBrushColor(hdc, PIXEL_blue);
        FillBox(hdc, rcDraw->left, rcDraw->right, RECTWP(rcDraw), RECTHP(rcDraw));
    }

    TextOut(hdc, (rcDraw->left + rcDraw->right) / 2 - 160, (rcDraw->top + rcDraw->bottom) / 2 - 20, label);
}

void TouchMenu::DrawLeft(HWND hWnd, GHANDLE hsvi, HDC hdc, RECT* rcDraw)
{
    MGScrollView* pThis = (MGScrollView*)GetWindowAdditionalData(hWnd);
    const char* label = (const char*)scrollview_get_item_adddata(hsvi);

    SetBkMode(hdc, BM_TRANSPARENT);
    SetTextColor(hdc, PIXEL_black);

    if (scrollview_is_item_hilight(hWnd, hsvi)) {
        SetBrushColor(hdc, PIXEL_blue);
        FillBox(hdc, rcDraw->left, rcDraw->right, RECTWP(rcDraw), RECTHP(rcDraw));
    }

    TextOut(hdc, rcDraw->left + 16, (rcDraw->top + rcDraw->bottom) / 2 - 20, label);
}