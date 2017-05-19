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
#include "tpresource.h"

const static char setting_img[] = "setting.bmp";
const static char explorer_img[] = "explorer.bmp";
const static char preview_img[] = "preview.bmp";
const static char captrue_img[] = "captrue.bmp";
const static char sleep_img[] = "sleep.bmp";
const static char return_img[] = "return.bmp";
const static char play_img[] = "play.bmp";
const static char audio_img[2][20] = { "audio_on.bmp", "audio_off.bmp" };
const static char lock_img[2][20] = { "lock_off.bmp", "lock_on.bmp" };
const static char record_img[2][20] = { "record.bmp", "record_on.bmp" };

const static char system_setting_img[2][30] = { "system_setting_off.bmp", "system_setting_on.bmp" };
const static char video_setting_img[2][30] = { "video_setting_off.bmp", "video_setting_on.bmp" };
const static char back_img[2][20] = { "back_off.bmp", "back_on.bmp" };
const static char front_img[2][20] = { "front_off.bmp", "front_on.bmp" };
const static char video_explorer_img[2][30] = { "video_explorer_off.bmp", "video_explorer_on.bmp" };
const static char lock_explorer_img[2][30] = { "lock_explorer_off.bmp", "lock_explorer_on.bmp" };
const static char photo_explorer_img[2][30] = { "photo_explorer_off.bmp", "photo_explorer_on.bmp" };

const static char wait_anim[] = "waitfor2.gif";

TPResource::TPResource()
{
    unsigned int i;
    char img[128];
    char respath[] = "/usr/local/share/minigui/res/images/";

    sprintf(img, "%s%s", respath, setting_img);
    if (LoadBitmap(HDC_SCREEN, &bmp_setting, img))
        goto err;

    sprintf(img, "%s%s", respath, explorer_img);
    if (LoadBitmap(HDC_SCREEN, &bmp_explorer, img))
        goto err;

    sprintf(img, "%s%s", respath, preview_img);
    if (LoadBitmap(HDC_SCREEN, &bmp_preview, img))
        goto err;

    sprintf(img, "%s%s", respath, captrue_img);
    if (LoadBitmap(HDC_SCREEN, &bmp_captrue, img))
        goto err;

    sprintf(img, "%s%s", respath, sleep_img);
    if (LoadBitmap(HDC_SCREEN, &bmp_sleep, img))
        goto err;

    sprintf(img, "%s%s", respath, return_img);
    if (LoadBitmap(HDC_SCREEN, &bmp_return, img))
        goto err;

    sprintf(img, "%s%s", respath, play_img);
    if (LoadBitmap(HDC_SCREEN, &bmp_play, img))
        goto err;

    for (i = 0; i < (sizeof(bmp_audio) / sizeof(BITMAP)); i++) {
        sprintf(img, "%s%s", respath, audio_img[i]);
        if (LoadBitmap(HDC_SCREEN, &bmp_audio[i], img))
            goto err;
    }

    for (i = 0; i < (sizeof(bmp_lock) / sizeof(BITMAP)); i++) {
        sprintf(img, "%s%s", respath, lock_img[i]);
        if (LoadBitmap(HDC_SCREEN, &bmp_lock[i], img))
            goto err;
    }

    for (i = 0; i < (sizeof(bmp_record) / sizeof(BITMAP)); i++) {
        sprintf(img, "%s%s", respath, record_img[i]);
        if (LoadBitmap(HDC_SCREEN, &bmp_record[i], img))
            goto err;
    }

    for (i = 0; i < (sizeof(bmp_system_setting) / sizeof(BITMAP)); i++) {
        sprintf(img, "%s%s", respath, system_setting_img[i]);
        if (LoadBitmap(HDC_SCREEN, &bmp_system_setting[i], img))
            goto err;
    }

    for (i = 0; i < (sizeof(bmp_video_setting) / sizeof(BITMAP)); i++) {
        sprintf(img, "%s%s", respath, video_setting_img[i]);
        if (LoadBitmap(HDC_SCREEN, &bmp_video_setting[i], img))
            goto err;
    }

    for (i = 0; i < (sizeof(bmp_front) / sizeof(BITMAP)); i++) {
        sprintf(img, "%s%s", respath, front_img[i]);
        if (LoadBitmap(HDC_SCREEN, &bmp_front[i], img))
            goto err;
    }

    for (i = 0; i < (sizeof(bmp_back) / sizeof(BITMAP)); i++) {
        sprintf(img, "%s%s", respath, back_img[i]);
        if (LoadBitmap(HDC_SCREEN, &bmp_back[i], img))
            goto err;
    }

    for (i = 0; i < (sizeof(bmp_video_explorer) / sizeof(BITMAP)); i++) {
        sprintf(img, "%s%s", respath, video_explorer_img[i]);
        if (LoadBitmap(HDC_SCREEN, &bmp_video_explorer[i], img))
            goto err;
    }

    for (i = 0; i < (sizeof(bmp_lock_explorer) / sizeof(BITMAP)); i++) {
        sprintf(img, "%s%s", respath, lock_explorer_img[i]);
        if (LoadBitmap(HDC_SCREEN, &bmp_lock_explorer[i], img))
            goto err;
    }

    for (i = 0; i < (sizeof(bmp_photo_explorer) / sizeof(BITMAP)); i++) {
        sprintf(img, "%s%s", respath, photo_explorer_img[i]);
        if (LoadBitmap(HDC_SCREEN, &bmp_photo_explorer[i], img))
            goto err;
    }

    sprintf(img, "%s%s", respath, wait_anim);
    anim_wait = CreateAnimationFromGIF89aFile(HDC_SCREEN, img);

    return;

err:
    printf("loadres fail\n");
}

TPResource::~TPResource()
{
    unsigned int i;

    UnloadBitmap(&bmp_setting);

    UnloadBitmap(&bmp_explorer);

    UnloadBitmap(&bmp_preview);

    UnloadBitmap(&bmp_captrue);

    UnloadBitmap(&bmp_sleep);

    UnloadBitmap(&bmp_return);

    UnloadBitmap(&bmp_play);

    for (i = 0; i < (sizeof(bmp_audio) / sizeof(BITMAP)); i++) {
        UnloadBitmap(&bmp_audio[i]);
    }

    for (i = 0; i < (sizeof(bmp_lock) / sizeof(BITMAP)); i++) {
        UnloadBitmap(&bmp_lock[i]);
    }

    for (i = 0; i < (sizeof(bmp_record) / sizeof(BITMAP)); i++) {
        UnloadBitmap(&bmp_record[i]);
    }

    for (i = 0; i < (sizeof(bmp_video_explorer) / sizeof(BITMAP)); i++) {
        UnloadBitmap(&bmp_video_explorer[i]);
    }

    for (i = 0; i < (sizeof(bmp_lock_explorer) / sizeof(BITMAP)); i++) {
        UnloadBitmap(&bmp_lock_explorer[i]);
    }

    for (i = 0; i < (sizeof(bmp_photo_explorer) / sizeof(BITMAP)); i++) {
        UnloadBitmap(&bmp_photo_explorer[i]);
    }

    DestroyAnimation(anim_wait, TRUE);

    return;
}