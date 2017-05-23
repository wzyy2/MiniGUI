#ifndef TOUCHMENU_H
#define TOUCHMENU_H

#include "tpcommon.h"
#include "widget/touchscrollwindow.h"
#include <map>

class MenuNotification;

class TouchMenu : public TouchScrollWnd {
    friend class MenuNotification;
    typedef void (*OPTIONFUNC)(TouchMenu* parent, MGWnd* sender);

public:
    TouchMenu(HWND hWndParent, int x, int y, int w, int h, DWORD dwStyle,
        DWORD dwStylEx = 0, int Id = -1, int left = 0);
    ~TouchMenu();

    BOOL AddSetting(const char* desc, OPTIONFUNC callback);

    inline void AddOtion(MGWnd* option)
    {
        left_options.push_back(option);
        SetContHeight(left_options.size() * _default_height);
    }

    inline int GetOptionSize()
    {
        return left_options.size();
    }

    inline void SetDefaultHeight(int val)
    {
        _default_height = val;
    }

    inline int GetDefaultHeight()
    {
        return _default_height;
    }

private:
    MenuNotification* menu_notification = NULL;
    TouchMenu* left_menu = NULL;
    std::vector<MGWnd*> settings;
    std::vector<MGWnd*> left_options;
    std::map<MGWnd*, OPTIONFUNC> options_create;

    int _default_height = 100;
    unsigned int _left, _x, _y, _w, _h, _dwStyle;
};

#endif
