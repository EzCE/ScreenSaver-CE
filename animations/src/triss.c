#include <graphx.h>
#include <keypadc.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "utility.h"
#include "palette.h"

#define NUM_TRIS 15

typedef struct Triangle {
    uint16_t x1, y1;
    uint16_t x2, y2;
    uint16_t x3, y3;
    uint16_t color;
} Triangle;

void triss_init(const uint8_t numTris, Triangle tris[numTris]) {
    tris[0].x1 = random() % GFX_LCD_WIDTH;
    tris[0].y1 = random() % GFX_LCD_HEIGHT;
    tris[0].x2 = random() % GFX_LCD_WIDTH;
    tris[0].y2 = random() % GFX_LCD_HEIGHT;
    tris[0].x3 = random() % GFX_LCD_WIDTH;
    tris[0].y3 = random() % GFX_LCD_HEIGHT;
    tris[0].color = random() % 255 + 1;
    
    for (uint8_t i = 1; i < numTris; i++) {
        tris[i].x1 = tris[0].x1;
        tris[i].y1 = tris[0].y1;
        tris[i].x2 = tris[0].x2;
        tris[i].y2 = tris[0].y2;
        tris[i].x3 = tris[0].x3;
        tris[i].y3 = tris[0].y3;
        tris[i].color = tris[0].color;
    }
}

void triss_draw(const uint8_t numTris, Triangle tris[numTris]) {
    for (int8_t i = numTris - 1; i >= 0; i--) {
        gfx_SetColor(tris[i].color);
        gfx_Line(tris[i].x1, tris[i].y1, tris[i].x2, tris[i].y2);
        gfx_Line(tris[i].x2, tris[i].y2, tris[i].x3, tris[i].y3);
        gfx_Line(tris[i].x3, tris[i].y3, tris[i].x1, tris[i].y1);
    }
}

void update_point(uint16_t *x, uint16_t *y, int16_t *dx, int16_t *dy) {
    if (*x + *dx >= GFX_LCD_WIDTH || *x + *dx <= 0) 
        *dx = -*dx;
    *x += *dx;

    if (*y + *dy >= GFX_LCD_HEIGHT || *y + *dy <= 0) 
        *dy = -*dy;
    *y += *dy;
}

void triss_update(const uint8_t numTris, Triangle tris[numTris], 
    int16_t *dx1, int16_t *dy1, 
    int16_t *dx2, int16_t *dy2, 
    int16_t *dx3, int16_t *dy3) {

    for (int i = numTris - 1; i > 0; i--) {
        tris[i].x1 = tris[i - 1].x1;
        tris[i].y1 = tris[i - 1].y1;
        tris[i].x2 = tris[i - 1].x2;
        tris[i].y2 = tris[i - 1].y2;
        tris[i].x3 = tris[i - 1].x3;
        tris[i].y3 = tris[i - 1].y3;
        tris[i].color = tris[i - 1].color - 10;
    }

    update_point(&tris[0].x1, &tris[0].y1, dx1, dy1);
    update_point(&tris[0].x2, &tris[0].y2, dx2, dy2);
    update_point(&tris[0].x3, &tris[0].y3, dx3, dy3);
}

bool triss(void) {
    gfx_Begin();
    gfx_FillScreen(0);

    gfx_palette[0] = 0;
    palette_rainbow(gfx_palette);

    srand(rtc_Time());
    Triangle tris[NUM_TRIS];
    triss_init(NUM_TRIS, tris);

    gfx_SetDrawScreen();

    int16_t dx1 = random() % 4 + 1; 
    int16_t dy1 = random() % 4 + 1; 
    
    int16_t dx2 = random() % 4 + 1; 
    int16_t dy2 = random() % 4 + 1; 
    
    int16_t dx3 = random() % 4 + 1; 
    int16_t dy3 = random() % 4 + 1;

    while (!kb_AnyKey()) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }

        gfx_ZeroScreen();

        triss_draw(NUM_TRIS, tris);
        triss_update(NUM_TRIS, tris, &dx1, &dy1, &dx2, &dy2, &dx3, &dy3);
        palette_shift(gfx_palette);
        
        gfx_SwapDraw();
    }

    gfx_End();
    return false;
}
