#include <graphx.h>
#include <keypadc.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "utility.h"

#define NUM_STARS 150
#define MAX_DEPTH 256
#define CENTER_X (GFX_LCD_WIDTH / 2)
#define CENTER_Y (GFX_LCD_HEIGHT / 2)
#define SPEED 7

#define SCREEN_X(x, z) (((int32_t)(x) * 128 / (z)) + CENTER_X)
#define SCREEN_Y(y, z) (((int32_t)(y) * 128 / (z)) + CENTER_Y)

typedef struct Star {
    int16_t x;
    int16_t y;
    int16_t z;
} Star;

static void star_reset(Star *star) {
    star->x = (random() % GFX_LCD_WIDTH) - CENTER_X;
    star->y = (random() % GFX_LCD_HEIGHT) - CENTER_Y;
    star->z = MAX_DEPTH;
}

static void starfield_init(const uint8_t numStars, Star stars[numStars]) {
    for (uint8_t i = 0; i < numStars; i++) {
        star_reset(&stars[i]);
        stars[i].z = (random() % (MAX_DEPTH - 1)) + 1;
    }
}

static bool starfield_update(const uint8_t numStars, Star stars[numStars]) {
    for (uint8_t i = 0; i < numStars; i++) {
        Star *star = &stars[i];
        
        star->z -= SPEED;
        if (star->z <= 0)
            star_reset(star);
    }

    return !kb_AnyKey();
}

static void draw_streak(Star *star, int16_t screenX, int16_t screenY) {
    int16_t prevZ = star->z + SPEED * 2;
    if (prevZ > MAX_DEPTH) 
        prevZ = MAX_DEPTH;
    
    int16_t prevX = SCREEN_X(star->x, prevZ);
    int16_t prevY = SCREEN_Y(star->y, prevZ);
    
    if (prevX >= 0 && prevX < GFX_LCD_WIDTH &&
        prevY >= 0 && prevY < GFX_LCD_HEIGHT) {
        gfx_SetColor((256 - star->z) / 2);
        gfx_Line(prevX, prevY, screenX, screenY);
    }
}

static void starfield_draw(const uint8_t numStars, Star stars[numStars]) {
    for (uint8_t i = 0; i < numStars; i++) {
        Star *star = &stars[i];
        
        int16_t screenX = SCREEN_X(star->x, star->z);
        int16_t screenY = SCREEN_Y(star->y, star->z);
        
        if (screenX < 0 || screenX >= GFX_LCD_WIDTH ||
            screenY < 0 || screenY >= GFX_LCD_HEIGHT) {
            continue;
        }
        
        if (star->z < SPEED * 18)
            draw_streak(star, screenX, screenY);

        gfx_SetColor(256 - star->z);
        
        int16_t z = star->z;
        if (z > SPEED * 20)
            gfx_SetPixel(screenX, screenY);
        else if (z > SPEED * 8)
            gfx_FillRectangle(screenX - 1, screenY - 1, 3, 3);
        else
          gfx_FillCircle(screenX, screenY, 2);
    }
}

static void palette_grayscale(uint16_t* palette) {
    for (uint16_t i = 0; i < 256; i++) {
        palette[i] = gfx_RGBTo1555(i, i, i);
    }
}

int main(void) {
    gfx_Begin();
    gfx_ZeroScreen();
    
    palette_grayscale(gfx_palette);
    
    srand(rtc_Time());
    
    Star stars[NUM_STARS];
    starfield_init(NUM_STARS, stars);
    
    gfx_SetDrawBuffer();
    
    while (starfield_update(NUM_STARS, stars)) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }
        
        gfx_ZeroScreen();
        starfield_draw(NUM_STARS, stars);
        
        gfx_SwapDraw();
    }
    
    gfx_End();
    return false;
}
