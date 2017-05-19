#ifndef __UI_854X480_H__
#define __UI_854X480_H__

#define WIN_W	854
#define WIN_H	480

#define MAIN_BTN_W 120

#define SIDE_W 160

#define RETURN_H 60

#define SETTING_ITEM_H 60

#define EXPLORER_ITEM_R 2
#define EXPLORER_ITEM_C 4

#define THEME_COLOR 0x632c



#define VIDEO_DISPLAY_ROTATE_ANGLE 270

#define A_IMG_X 265
#define A_IMG_Y 20
#define A_IMG_W 37
#define A_IMG_H 20

#define adas_X	0
#define adas_Y	61
#define adas_W	WATERMARK_TIME_X
#define adas_H	WIN_H

#define MOVE_IMG_X	353
#define MOVE_IMG_Y	8
#define MOVE_IMG_W	43
#define MOVE_IMG_H	44

#define USB_IMG_X 290
#define USB_IMG_Y 117
#define USB_IMG_W 276
#define USB_IMG_H 243

#define BATT_IMG_X (WIN_W - 60)
#define BATT_IMG_Y 19
#define BATT_IMG_W 43
#define BATT_IMG_H 21

#define TF_IMG_X (WIN_W - 120)
#define TF_IMG_Y 9
#define TF_IMG_W 43
#define TF_IMG_H 42

#define MODE_IMG_X 0
#define MODE_IMG_Y 0
#define MODE_IMG_W 130
#define MODE_IMG_H 61

#define RESOLUTION_IMG_X 161
#define RESOLUTION_IMG_Y 19
#define RESOLUTION_IMG_W 79
#define RESOLUTION_IMG_H 22

#define TOPBK_IMG_X 0
#define TOPBK_IMG_Y 0
#define TOPBK_IMG_W 1
#define TOPBK_IMG_H 61

#define REC_TIME_X 70
#define REC_TIME_Y (WIN_H - 100)
#define REC_TIME_W 300
#define REC_TIME_H 50

#define REC_IMG_X 41
#define REC_IMG_Y (WIN_H - 100)
#define REC_IMG_W 14
#define REC_IMG_H 14

/* Start 16-byte alignment*/
#define WATERMARK_TIME_W    160
#define WATERMARK_TIME_H    16
#define WATERMARK_TIME_X    ((WIN_W - WATERMARK_TIME_W) & 0xfffffff0) //688
#define WATERMARK_TIME_Y    ((TOPBK_IMG_H + 15) & 0xfffffff0) //64

#define WATERMARK_IMG_W    96
#define WATERMARK_IMG_H    32
#define WATERMARK_IMG_X    ((WIN_W - WATERMARK_IMG_W - 16) & 0xfffffff0) //736
#define WATERMARK_IMG_Y    (WATERMARK_TIME_Y + WATERMARK_TIME_H) //80

#define WATERMARK_LICN_W    80
#define WATERMARK_LICN_H    16
#define WATERMARK_LICN_X    ((WIN_W - WATERMARK_LICN_W) & 0xfffffff0) //688
#define WATERMARK_LICN_Y    (WATERMARK_IMG_Y + WATERMARK_IMG_H) //80
/* End 16-byte alignment */

#define MIC_IMG_X (WIN_W - 100)
#define MIC_IMG_Y (WIN_H - 100)
#define MIC_IMG_W 34
#define MIC_IMG_H 39

#define TFCAP_STR_X (WIN_W - 220)
#define TFCAP_STR_Y 22
#define TFCAP_STR_W 100
#define TFCAP_STR_H 42

#define SYSTIME_STR_X (WIN_W - 400)
#define SYSTIME_STR_Y 22
#define SYSTIME_STR_W WATERMARK_TIME_W
#define SYSTIME_STR_H WATERMARK_TIME_H

#define FILENAME_STR_X 50
#define FILENAME_STR_Y (WIN_H - 80)
#define FILENAME_STR_W 400
#define FILENAME_STR_H 42

#define PLAY_IMG_W 64
#define PLAY_IMG_H 64

#define WIFI_IMG_X (WIN_W - 180)
#define WIFI_IMG_Y (WIN_H - 97)
#define WIFI_IMG_W 48
#define WIFI_IMG_H 33


#endif
