#ifndef TOUCHMENU_H
#define TOUCHMENU_H

#include "tpcommon.h"
#include <vector>

class TouchMenu : public MGScrollView {
protected:
    MGCtrlNotification* m_notification;
    static void _ctrl_notification(HWND hWnd, int id, int code, DWORD add_data)
    {
        TouchMenu *_left, *_right;
        TouchMenu* _this = (TouchMenu*)WndFromHandle(hWnd);

        if (_this && _this->left_menu) {
            _left = _this->left_menu;
            _right = _this;

            if (code == SVN_SELCHANGED) {
                // flush left menu option
                _right->FlushOption(_right->GetIndex(add_data));
            }
        } else {
            _left = _this;
            _right = (TouchMenu*)WndFromHandle(_this->GetParent());

            // tell which option is choosed
            if (_right->m_notification)
                _right->m_notification->OnCtrlNotified(_right, id, code,
                    _left->GetIndex(add_data));
        }
    }

public:
    TouchMenu(HWND hWndParent, int x, int y, int w, int h, DWORD dwStyle,
        DWORD dwStylEx = 0, int Id = -1, int left = 0);
    ~TouchMenu();

    void CreateLeft(HWND hWndParent, int x, int y, int w, int h, DWORD dwStyle,
        DWORD dwStylEx = 0, int Id = -1);

    BOOL AddMenuItem(const char* desc);
    BOOL AddOption(int index, const char* desc);
    void FlushOption(int index);

    inline BOOL ShowWindow(int iCmdShow = SW_SHOWNORMAL)
    {
        if (left_menu)
            left_menu->ShowWindow(iCmdShow);

        return ::ShowWindow(m_hWnd, iCmdShow);
    }

    inline void SetDefaultItemHeight(int height) { _item_height = height; };

    inline int GetIndex(HSVITEM handle)
    {
        // get index from handle
        for (unsigned int i = 0; i < save_items.size(); i++) {
            if (handle == save_items[i].handle) {
                return i;
            }
        }
        return -1;
    };

    MGCtrlNotification* SetNotification(MGCtrlNotification* notification)
    {
        MGCtrlNotification* old_notif = m_notification;
        m_notification = notification;

        if (left_menu)
            left_menu->SetNotification(notification);

        if (IsWindow())
            ::SetNotificationCallback(m_hWnd, (NOTIFPROC)_ctrl_notification);
        return old_notif;
    }

private:
    TouchMenu* left_menu = NULL;
    int _item_height = 100;

    struct MenuElement {
        SVITEMINFO info;
        HSVITEM handle;

        std::vector<const char*> options;
    };

    std::vector<struct MenuElement> save_items;

    static void Draw(HWND hWnd, GHANDLE hsvi, HDC hdc, RECT* rcDraw);
    static void DrawLeft(HWND hWnd, GHANDLE hsvi, HDC hdc, RECT* rcDraw);
};

#endif
