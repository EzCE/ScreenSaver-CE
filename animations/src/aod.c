#include "utility.h"

#include <graphx.h>
#include <keypadc.h>

#include <math.h>
#include <sys/rtc.h>

#define M_PI 3.14159265358979323846

bool aod(void) {
    gfx_Begin();
    gfx_SetColor(255);
    gfx_SetDrawBuffer();
    gfx_ZeroScreen();

    uint8_t date[2];
    boot_GetDate(&date[0], &date[1], NULL);

    const char months[12][4] = {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec",
    };

    gfx_SetTextBGColor(0);
    gfx_SetTextFGColor(255);
    gfx_SetTextTransparentColor(0);
    gfx_FillRectangle_NoClip(160, 162, 3, 3);
    gfx_PrintStringXY(months[date[1] - 1], 167, 160);
    gfx_SetTextXY(194, 160);
    gfx_PrintUInt(date[0], 1);

    while (!kb_AnyKey()) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }

        gfx_SetColor(0);
        gfx_FillRectangle_NoClip(118, 73, 85, 85);
        gfx_FillRectangle_NoClip(122, 160, 33, 7);
        gfx_SetColor(255);
        gfx_FillCircle_NoClip(160, 114, 36);
        gfx_SetColor(0);
        gfx_FillCircle_NoClip(160, 114, 32);
        gfx_SetColor(192);
        gfx_Line_NoClip(160, 114, 160 + 41 * cos(((double)rtc_Seconds / 60) * 2 * M_PI - M_PI / 2), 114 + 41 * sin(((double)rtc_Seconds / 60) * 2 * M_PI - M_PI / 2));
        gfx_SetColor(255);
        gfx_Line_NoClip(160, 114, 160 + 41 * cos((((double)rtc_Minutes * 60 + (double)rtc_Seconds) / 3600) * 2 * M_PI - M_PI / 2), 114 + 41 * sin((((double)rtc_Minutes * 60 + (double)rtc_Seconds) / 3600) * 2 * M_PI - M_PI / 2));
        gfx_Line_NoClip(160, 114, 160 + 26 * cos((((double)rtc_Hours * 3600 + (double)rtc_Minutes * 60 + (double)rtc_Seconds) / 43200) * 2 * M_PI - M_PI / 2), 114 + 26 * sin((((double)rtc_Hours * 3600 + (double)rtc_Minutes * 60 + (double)rtc_Seconds) / 43200) * 2 * M_PI - M_PI / 2));
        gfx_SetTextXY((rtc_Hours < 10 ? 129 : 122), 160);
        gfx_PrintUInt(rtc_Hours, 1);
        gfx_PrintChar(':');
        gfx_PrintUInt(rtc_Minutes, 2);

        gfx_BlitBuffer();
    }

    gfx_End();

    return false;
}
