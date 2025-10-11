#include <graphx.h>
#include <keypadc.h>

#include <math.h>
#include <sys/rtc.h>

#define M_PI 3.14159265358979323846

void aod(void) {
    gfx_Begin();
    gfx_SetColor(255);
    gfx_SetDrawBuffer();

    uint16_t date[3];
    boot_GetDate(&date[0], &date[1], &date[2]);

    const char months[12] = {
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

    

    while (!kb_AnyKey()) {
        gfx_SetColor(0);
        gfx_FillRectangle_NoClip(118, 78, 85, 85);
        gfx_SetColor(255);
        gfx_FillCircle_NoClip(160, 110, 36);
        gfx_SetColor(0);
        gfx_FillCircle_NoClip(160, 110, 32);
        gfx_SetColor(255);
        // gfx_Line_NoClip(160, 110, 160 + 42 * cos(((double)rtc_Seconds / 60) * 2 * M_PI - M_PI / 2), 110 + 42 * sin(((double)rtc_Seconds / 60) * 2 * M_PI - M_PI / 2));
        gfx_Line_NoClip(160, 110, 160 + 42 * cos((((double)rtc_Minutes * 60 + (double)rtc_Seconds) / 3600) * 2 * M_PI - M_PI / 2), 110 + 42 * sin((((double)rtc_Minutes * 60 + (double)rtc_Seconds) / 3600) * 2 * M_PI - M_PI / 2));
        gfx_Line_NoClip(160, 110, 160 + 26 * cos((((double)rtc_Hours * 3600 + (double)rtc_Minutes * 60 + (double)rtc_Seconds) / 43200) * 2 * M_PI - M_PI / 2), 110 + 26 * sin((((double)rtc_Hours * 3600 + (double)rtc_Minutes * 60 + (double)rtc_Seconds) / 43200) * 2 * M_PI - M_PI / 2));
        gfx_BlitBuffer();
    }

    gfx_End();
}
