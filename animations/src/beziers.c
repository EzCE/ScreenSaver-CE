#include <graphx.h>
#include <keypadc.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "utility.h"
#include "palette.h"

#define NUM_CURVES 9
#define BEZIER_SEGMENTS 14

typedef struct Bezier {
    uint16_t x0, y0;
    uint16_t x1, y1;
    uint16_t x2, y2;
    uint16_t x3, y3;
    uint16_t x4, y4;
    uint16_t color;
} Bezier;

void bezier_init(const uint8_t numCurves, Bezier curves[numCurves]) {
    curves[0].x0 = random() % GFX_LCD_WIDTH;
    curves[0].y0 = random() % GFX_LCD_HEIGHT;
    curves[0].x1 = random() % GFX_LCD_WIDTH;
    curves[0].y1 = random() % GFX_LCD_HEIGHT;
    curves[0].x2 = random() % GFX_LCD_WIDTH;
    curves[0].y2 = random() % GFX_LCD_HEIGHT;
    curves[0].x3 = random() % GFX_LCD_WIDTH;
    curves[0].y3 = random() % GFX_LCD_HEIGHT;
    curves[0].x4 = random() % GFX_LCD_WIDTH;
    curves[0].y4 = random() % GFX_LCD_HEIGHT;
    curves[0].color = random() % 255 + 1;
    
    for (uint8_t i = 1; i < numCurves; i++) {
        curves[i].x0 = curves[0].x0;
        curves[i].y0 = curves[0].y0;
        curves[i].x1 = curves[0].x1;
        curves[i].y1 = curves[0].y1;
        curves[i].x2 = curves[0].x2;
        curves[i].y2 = curves[0].y2;
        curves[i].x3 = curves[0].x3;
        curves[i].y3 = curves[0].y3;
        curves[i].x4 = curves[0].x4;
        curves[i].y4 = curves[0].y4;
        curves[i].color = curves[0].color;
    }
}

void bezier_draw_curve(const Bezier *curve) {
    int16_t prevX = curve->x0;
    int16_t prevY = curve->y0;
    
    for (uint8_t i = 1; i <= BEZIER_SEGMENTS; i++) {
        uint16_t t = (i * 256) / BEZIER_SEGMENTS; 
        uint16_t t2 = (t * t) >> 8;
        uint16_t t3 = (t2 * t) >> 8;
        uint16_t t4 = (t3 * t) >> 8;
        uint16_t mt = 256 - t;
        uint16_t mt2 = (mt * mt) >> 8;
        uint16_t mt3 = (mt2 * mt) >> 8;
        uint16_t mt4 = (mt3 * mt) >> 8;
        
        uint16_t b0 = mt4;
        uint16_t b1 = (4 * ((mt3 * t) >> 8));
        uint16_t b2 = (6 * ((mt2 * t2) >> 8));
        uint16_t b3 = (4 * ((mt * t3) >> 8));
        uint16_t b4 = t4;
        
        int16_t x = (b0 * curve->x0 + b1 * curve->x1 + b2 * curve->x2 + b3 * curve->x3 + b4 * curve->x4) >> 8;
        int16_t y = (b0 * curve->y0 + b1 * curve->y1 + b2 * curve->y2 + b3 * curve->y3 + b4 * curve->y4) >> 8;
        
        gfx_Line(prevX, prevY, x, y);
        prevX = x;
        prevY = y;
    }
    
    gfx_Line(prevX, prevY, curve->x0, curve->y0);
}

void bezier_draw(const uint8_t numCurves, Bezier curves[numCurves]) {
    for (int8_t i = numCurves - 1; i >= 0; i--) {
        gfx_SetColor(curves[i].color);
        bezier_draw_curve(&curves[i]);
    }
}

void bezier_update_point(uint16_t *x, uint16_t *y, int16_t *dx, int16_t *dy) {
    if (*x + *dx >= GFX_LCD_WIDTH || *x + *dx <= 0) 
        *dx = -*dx;
    *x += *dx;

    if (*y + *dy >= GFX_LCD_HEIGHT || *y + *dy <= 0) 
        *dy = -*dy;
    *y += *dy;
}

void bezier_update(const uint8_t numCurves, Bezier curves[numCurves], 
    int16_t *dx0, int16_t *dy0, 
    int16_t *dx1, int16_t *dy1, 
    int16_t *dx2, int16_t *dy2,
    int16_t *dx3, int16_t *dy3,
    int16_t *dx4, int16_t *dy4) {

    for (int i = numCurves - 1; i > 0; i--) {
        curves[i].x0 = curves[i - 1].x0;
        curves[i].y0 = curves[i - 1].y0;
        curves[i].x1 = curves[i - 1].x1;
        curves[i].y1 = curves[i - 1].y1;
        curves[i].x2 = curves[i - 1].x2;
        curves[i].y2 = curves[i - 1].y2;
        curves[i].x3 = curves[i - 1].x3;
        curves[i].y3 = curves[i - 1].y3;
        curves[i].x4 = curves[i - 1].x4;
        curves[i].y4 = curves[i - 1].y4;
        curves[i].color = curves[i - 1].color - 10;
    }

    bezier_update_point(&curves[0].x0, &curves[0].y0, dx0, dy0);
    bezier_update_point(&curves[0].x1, &curves[0].y1, dx1, dy1);
    bezier_update_point(&curves[0].x2, &curves[0].y2, dx2, dy2);
    bezier_update_point(&curves[0].x3, &curves[0].y3, dx3, dy3);
    bezier_update_point(&curves[0].x4, &curves[0].y4, dx4, dy4);
}

bool beziers(void) {
    gfx_Begin();
    gfx_FillScreen(0);

    gfx_palette[0] = 0;
    palette_rainbow(gfx_palette);

    srand(rtc_Time());
    Bezier curves[NUM_CURVES];
    bezier_init(NUM_CURVES, curves);

    gfx_SetDrawScreen();

    int16_t dx0 = (random() % 4 + 1) * 2; 
    int16_t dy0 = (random() % 4 + 1) * 2; 
    
    int16_t dx1 = (random() % 4 + 1) * 2; 
    int16_t dy1 = (random() % 4 + 1) * 2; 
    
    int16_t dx2 = (random() % 4 + 1) * 2; 
    int16_t dy2 = (random() % 4 + 1) * 2;

    int16_t dx3 = (random() % 4 + 1) * 2; 
    int16_t dy3 = (random() % 4 + 1) * 2;

    int16_t dx4 = (random() % 4 + 1) * 2; 
    int16_t dy4 = (random() % 4 + 1) * 2;

    while (!kb_AnyKey()) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }

        gfx_ZeroScreen();

        bezier_draw(NUM_CURVES, curves);
        bezier_update(NUM_CURVES, curves, &dx0, &dy0, &dx1, &dy1, &dx2, &dy2, &dx3, &dy3, &dx4, &dy4);
        palette_shift(gfx_palette);
        
        gfx_SwapDraw();
    }

    gfx_End();
    return false;
}
