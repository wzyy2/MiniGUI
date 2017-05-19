#ifndef SCROLLEXPLORER_H
#define SCROLLEXPLORER_H

#include "tpcommon.h"

class ScrollExplorer : public MGScrollView {
public:
    ScrollExplorer(HWND hWndParent, int x, int y, int w, int h, DWORD dwStyle,
        DWORD dwStylEx = 0, int Id = -1);
    ~ScrollExplorer();

private:
    std::vector<MGStatic> video_items;

    void ShowPreview();
};

#endif
