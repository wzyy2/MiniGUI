#ifndef TPUTILS_H
#define TPUTILS_H

#include "tpcommon.h"
#include "tpresource.h"

namespace TPUtils {
enum {
    UI_FORMAT_BGRA_8888 = 0x1000,
    UI_FORMAT_RGB_565,
};

// define

// function
BOOL Init();
BOOL Exit();

void SetColorkey(DWORD bkColor);
int GetColorkey();

TPResource* GetResource();

int Suspend();

};

#endif
