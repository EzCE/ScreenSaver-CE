#include <graphx.h>
#include <keypadc.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "utility.h"
#include "gfx/gfx.h"

static void merth_update_point(uint16_t *x, uint16_t *y, int16_t *dx, int16_t *dy) {
    if (*x + *dx >= GFX_LCD_WIDTH - merth_face_width || *x + *dx <= 0) 
        *dx = -*dx;
    *x += *dx;

    if (*y + *dy >= GFX_LCD_HEIGHT - merth_face_height || *y + *dy <= 0) 
        *dy = -*dy;
    *y += *dy;
}

int main(void) {
    gfx_Begin();
    gfx_FillScreen(0);

    gfx_SetPalette(palette_merth, sizeof_palette_merth, merth_sprite_palette_offset);

    srand(rtc_Time());
    
    uint16_t x = random() % (GFX_LCD_WIDTH - merth_face_width);
    uint16_t y = random() % (GFX_LCD_HEIGHT - merth_face_height);

    gfx_SetDrawScreen();

    int16_t dx = 2;
    int16_t dy = 2;

    while (!kb_AnyKey()) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }

        gfx_ZeroScreen();

        gfx_Sprite(merth_face, x, y);
        gfx_SwapDraw();

        merth_update_point(&x, &y, &dx, &dy);
    }

    gfx_End();
    return 0;
}
