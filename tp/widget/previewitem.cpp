#include "previewitem.h"


PreviewItem::PreviewItem(BITMAP* resource, int colorkey, HWND hWndParent, int x, int y, int w, int h)
    : RKButton(hWndParent, x, y, w, h)
{
    SetWindowBkColor(colorkey);
    SetTristate(resource, NULL, NULL);
}


PreviewItem::~PreviewItem()
{
    
}
