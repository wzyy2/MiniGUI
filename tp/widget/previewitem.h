#ifndef PREVIEWITEM_H
#define PREVIEWITEM_H

#include "rkbutton.h"

class PreviewItem : public RKButton {
public:
    PreviewItem(BITMAP* resource, int colorkey, HWND hWndParent, int x, int y, int w, int h);
    ~PreviewItem();
};

#endif
