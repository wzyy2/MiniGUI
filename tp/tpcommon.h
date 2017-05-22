#ifndef TPCOMMON_H
#define TPCOMMON_H

// DEFINE
#define FILE_TYPE_MAX 3

// MGFC
#include "ui/cpp/mgcomm.h"
#include "ui/cpp/mgctrl.h"
#include "ui/cpp/mgwnd.h"

#include <stdexcept>
#include <vector>

// header mess copy from camera_ui
extern "C" {
#include "audio_dev.h"
#include "audioplay.h"
#include "collision/collision.h"
#include "common.h"
#include "example/user.h"
#include "fs_manage/fs_manage.h"
#include "fs_manage/fs_sdcard.h"
#include "fs_manage/fs_storage.h"
#include "fwk_protocol/rk_fwk.h"
#include "fwk_protocol/rk_protocol.h"
#include "gsensor.h"
#include "parameter.h"
#include "parking_monitor/parking_monitor.h"
#include "power/thermal.h"
#include "power_manage.h"
#include "storage/storage.h"
#include "ueventmonitor/ueventmonitor.h"
#include "ueventmonitor/usb_sd_ctrl.h"
#include "ui_resolution.h"
#include "video.h"
#include "videoplay.h"
#include "videoplay.h"
#include "watermark.h"
#include "wifi_management.h"
#include <assert.h>
#include <dirent.h>
#include <dpp/adas.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/watchdog.h>
#include <minigui/common.h>
#include <minigui/control.h>
#include <minigui/gdi.h>
#include <minigui/minigui.h>
#include <minigui/window.h>
#include <pthread.h>
#include <pwd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// HACK: unexport C function
int rkfb_get_pixel_format(void);
int rkfb_set_color_key(struct color_key* key);

int gsensor_init(void);
int gsensor_enable(int enable);
void gsensor_release(void);

char parameter_get_collision_level(void);
char parameter_get_leavecarrec(void);
char parameter_get_video_lan(void);

void audio_dev_deinit();

int fs_manage_deinit(void);
}

#endif