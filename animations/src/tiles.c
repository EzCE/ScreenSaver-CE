#include "gfx/gfxtiles.h"

#include <graphx.h>
#include <keypadc.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

void flip(int x, int y) {
    gfx_TempSprite(bg, 22, 38);

    static const int8_t leftX[10] = {
        0, -1, -1, -1, -1, 0, 1, 3, 5, 7,
    };

    static const int8_t leftY[10] = {
        0, -1, -1, -2, -3, -4, -5, -5, -6, -7,
    };

    static const int8_t rightX[10] = {
        0, -1, -2, -3, -3, -5, -6, -7, -8, -9,
    };

    static const int8_t rightY[10] = {
        0, 1, 1, 1, 2, 2, 2, 2, 2, 1,
    };

    uint8_t color1 = gfx_GetPixel(x, y);
    uint8_t color2 = randInt(0, 255);

    while (color2 == color1) {
        color2 = randInt(0, 255);
    }

    gfx_SetColor(0);
    gfx_FillRectangle(x, y, 20, 20);
    gfx_GetSprite_NoClip(bg, x - 1, y - 9);

    gfx_SetColor(color1);

    for (uint8_t f = 0; f < 10; f++) {
        int x1 = x + leftX[f];
        int x2 = x + 19 + rightX[f];
        int y1 = y + leftY[f];
        int y2 = y + rightY[f];
        int y3 = y + 19 - rightY[f];
        int y4 = y + 19 - leftY[f];

        gfx_Sprite(bg, x - 1, y - 9);
        gfx_FillTriangle(x1, y1, x2, y2, x2, y3);
        gfx_FillTriangle(x2, y3, x1, y4, x1, y1);
        gfx_BlitBuffer();
    }

    gfx_SetColor(color2);

    for (int8_t f = 9; f > -1; f--) {
        int x1 = x + 19 + rightX[f];
        int x2 = x + leftX[f];
        int y1 = y + leftY[f];
        int y2 = y + rightY[f];
        int y3 = y + 19 - rightY[f];
        int y4 = y + 19 - leftY[f];

        gfx_Sprite(bg, x - 1, y - 9);
        gfx_FillTriangle(x1, y1, x2, y2, x2, y3);
        gfx_FillTriangle(x2, y3, x1, y4, x1, y1);
        gfx_BlitBuffer();
    }
}

void tiles(void) {
    gfx_Begin();
    // gfx_SetPalette(palette_tiles, sizeof_palette_tiles, 0);
    gfx_ZeroScreen();
    srand(rtc_Time());
    gfx_SetDrawBuffer();

    while (!kb_AnyKey()) {
        int x = randInt(0, 15) * 20;
        int y = randInt(0, 11) * 20;

        gfx_BlitScreen();
        flip(x, y);
    }

    gfx_End();
}
