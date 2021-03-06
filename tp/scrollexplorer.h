#ifndef SCROLLEXPLORER_H
#define SCROLLEXPLORER_H

#include "tpcommon.h"
#include "widget/previewitem.h"
#include "widget/touchscrollwindow.h"

class ScrollExplorerButtonNotification;

class ScrollExplorer : public TouchScrollWnd {
    friend class ScrollExplorerButtonNotification;

public:
    ScrollExplorer(HWND hWndParent, int x, int y, int w, int h, DWORD dwStyle,
        DWORD dwStylEx = 0, int Id = 100);
    ~ScrollExplorer();

private:
    std::vector<PreviewItem*> video_items;
    ScrollExplorerButtonNotification* button_notification;

    void ShowPreview();
};

#endif
