#ifndef TPRESOURCE_H
#define TPRESOURCE_H

#include "tpcommon.h"

class TPResource {
public:
    TPResource();
    ~TPResource();

    BITMAP bmp_setting;
    BITMAP bmp_audio[2];
    BITMAP bmp_captrue;
    BITMAP bmp_lock[2];
    BITMAP bmp_record[2];
    BITMAP bmp_return;
    BITMAP bmp_sleep;
    BITMAP bmp_explorer;
    BITMAP bmp_preview;
    BITMAP bmp_play;

    BITMAP bmp_system_setting[2];
    BITMAP bmp_video_setting[2];
    BITMAP bmp_back[2];
    BITMAP bmp_front[2];

    BITMAP bmp_video_explorer[2];
    BITMAP bmp_lock_explorer[2];
    BITMAP bmp_photo_explorer[2];

    ANIMATION* anim_wait;

private:
};

#endif
