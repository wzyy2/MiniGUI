/**
 * Copyright (C) 2017 Fuzhou Rockchip Electronics Co., Ltd
 * Author: Jacob Chen <jacob2.chen@rock-chips.com>
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "tputils.h"

namespace TPUtils {
TPResource* tp_resource;
int color_key;

short screenoff_time;
void* keypress_sound;
void* capture_sound;

void RegisterCallback()
{
    // sd_reg_event_callback(sd_event_callback);
    // usb_reg_event_callback(usb_event_callback);
    // batt_reg_event_callback(batt_event_callback);
    // poweroff_reg_callback(poweroff_callback);
    // uevent_monitor_run();

    // //
    // REC_RegEventCallback(rec_event_callback);
    // USER_RegEventCallback(user_event_callback);
    // VIDEOPLAY_RegEventCallback(videoplay_event_callback);
    // hdmi_reg_event_callback(hdmi_event_callback);
    // cvbsout_reg_event_callback(cvbsout_event_callback);
    // camera_reg_event_callback(camere_event_callback);

    /*
     * If file size have been changed, need call the callball to nofity fs_manage
     * to change storage policy. Such as recordtime, resolution,
     * bit_rate_per_pixel, all they will effect file size.
     */
    // storage_setting_reg_event_callback(storage_setting_callback);
    // storage_setting_event_callback(0, NULL, NULL);

    // parking_regeventcallback(parking_event_callback);
    // collision_regeventcallback(collision_event_callback);
}

BOOL Init()
{
    char collision_level;
    char parkingrec = 0;

    tp_resource = new TPResource();

#ifdef USE_WATCHDOG
    char pathname[] = "/dev/watchdog";

    printf("use watchdog\n");
    fd_wtd = open(pathname, O_WRONLY);
    if (fd_wtd == -1)
        printf("/dev/watchdog open error\n");
    else {
        ioctl(fd_wtd, WDIOC_KEEPALIVE, 0);
    }
#endif

    android_usb_init();

    printf("camera run\n");
    video_record_init_lock();
    parameter_init();

    screenoff_time = parameter_get_screenoff_time();
    if (0 != audio_dev_init()) {
        printf("audio_dev_init failed\n");
    }

    // test code start
    // file path should be read from config_file, TODO
    // these files are copied from $ANDROID_PROJECT/frameworks/base/data/sounds
    if (audio_play_init(&keypress_sound,
            "/usr/local/share/sounds/KeypressStandard.wav", 1)) {
        printf("keypress sound init failed\n");
    }
    if (audio_play_init(&capture_sound,
            "/usr/local/share/sounds/camera_click.wav", 1)) {
        printf("capture sound init failed\n");
    }

    rk_fb_set_lcd_backlight(parameter_get_video_backlt());
    rk_fb_set_flip(parameter_get_video_flip());

    gsensor_init();
    thermal_init();
    gsensor_use_interrupt(GSENSOR_INT2, GSENSOR_INT_STOP);

    /* register collision get data function */
    collision_level = parameter_get_collision_level();
    collision_init();
    if (collision_level != 0)
        collision_register();

    /* register parkingrec get data function */
    parkingrec = parameter_get_leavecarrec();
    parking_init();
    if (parkingrec != 0)
        parking_register();

    TPUtils::RegisterCallback();

    // video_record_setaudio(parameter_get_video_audio());
    // initrec(hMainWnd);

    if (parameter_get_wifi_en() == 1)
        wifi_management_start();

// get_main_window_hwnd(hMainWnd);
// FlagForUI_ca.adasflagtooff = 0;
// FlagForUI_ca.formatflag = 0;
// FlagForUI_ca.setmode_ui = 0;
// FlagForUI_ca.sdcard_ui = sdcard;
// FlagForUI_ca.video_rec_state_ui = video_rec_state;

#ifdef MSG_FWK
    rk_fwk_glue_init();
    rk_fwk_controller_init();
#endif

#ifdef PROTOCOL_GB28181
    protocol_rk_gb28181_init();
#endif

#ifdef PROTOCOL_IOTC
    protocol_rk_iotc_init();
#endif

    // ui_init_uvc();

    // test code start
    audio_play_deinit(keypress_sound);
    audio_play_deinit(capture_sound);
    // test code end
    audio_dev_deinit();

    video_record_destroy_lock(); //must after uevent deinit
    printf("camera exit\n");

    return TRUE;
}

BOOL Exit()
{
    char parkingrec = 0;

#ifdef PROTOCOL_IOTC
    protocol_rk_iotc_destroy();
#endif

#ifdef PROTOCOL_GB28181
    protocol_rk_gb28181_destroy();
#endif

#ifdef MSG_FWK
    rk_fwk_controller_destroy();
    rk_fwk_glue_destroy();
#endif

    // ui_deinit_uvc();

    wifi_management_stop();
    // deinitrec(hMainWnd);
    video_record_deinit();
    videoplay_deinit();
    fs_manage_deinit();

    collision_unregister();

    parking_unregister();
    parkingrec = parameter_get_leavecarrec();
    if (parkingrec != 0) {
        gsensor_enable(1);
        gsensor_use_interrupt(GSENSOR_INT2, GSENSOR_INT_START);
    }
    gsensor_release();

    delete tp_resource;

    return TRUE;
}

void SetColorkey(DWORD bkColor)
{
    struct color_key key;

    key.blue = (bkColor & 0x1f) << 3;
    key.green = ((bkColor >> 5) & 0x3f) << 2;
    key.red = ((bkColor >> 11) & 0x1f) << 3;

    switch (rkfb_get_pixel_format()) {
    case UI_FORMAT_RGB_565:
        key.enable = 1;
        break;
    case UI_FORMAT_BGRA_8888:
    default:
        key.enable = 0;
        break;
    }

    if (rkfb_set_color_key(&key) == -1) {
        printf("rkfb_set_color_key err\n");
    }
    if (rkfb_set_color_key(&key) == -1) {
        printf("rkfb_set_color_key err\n");
    }
}

int GetColorkey()
{
    return color_key;
}

TPResource* GetResource()
{
    return tp_resource;
}

int Suspend()
{
    int ret;
    ret = system("echo 4 >> /sys/class/graphics/fb0/blank");
    ret = system("echo mem >> /sys/power/state");

    if (ret == 0)
        printf("Power_Sleep success\n");
    else
        printf("Power_Sleep failure ret=%d\n", ret);
}
}
