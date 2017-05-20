#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include "tpcommon.h"

class VideoSurface : public MGUserCtrl {
public:
    VideoSurface(int channel, int colorkey, HWND hWndParent, int x, int y, int w, int h,
        DWORD dwStyle, DWORD dwStylEx = 0, int Id = -1, int left = 0);
    ~VideoSurface();
    DECLARE_CTRL_CLASS("videosurface")

private:
    int _channel;

    BOOL WndProc(int iMsg, WPARAM wParam, LPARAM lParam, int* pret);
};

#endif
