#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "utility.h"

#define MAX_RAINDROPS       96
#define NUM_SIZES           15

#define BASE_SPEED          3.0f
#define SPEED_VARIANCE      2.0f

#define WIND_DRIFT          0.3f

#define RAND_PERCENT()      (randInt(0, 100) / 100.0f)

typedef struct Raindrop {
    float x;
    float y;
    float speedY;
    uint8_t length;
    float drift;
} Raindrop;

static void rain_initDrop(Raindrop* drop, bool startAtTop) {
    drop->length = randInt(3, NUM_SIZES + 6);
    
    drop->x = (float)randInt(0, GFX_LCD_WIDTH - 1);
    
    if (startAtTop) {
        drop->y = (float)(-drop->length) - randInt(0, 40);
    } else {
        drop->y = (float)randInt(0, GFX_LCD_HEIGHT - 1);
    }
    
    float sizeMultiplier = 0.7f + (drop->length * 0.1f);
    drop->speedY = BASE_SPEED + RAND_PERCENT() * SPEED_VARIANCE;
    drop->speedY *= sizeMultiplier;
    drop->drift = WIND_DRIFT * RAND_PERCENT();
}

static void rain_init(const uint8_t numDrops, Raindrop raindrops[numDrops]) {
    srand(rtc_Time());
    
    for (uint8_t i = 0; i < numDrops; i++) {
        rain_initDrop(&raindrops[i], false);
    }
}

static bool rain_update(const uint8_t numDrops, Raindrop raindrops[numDrops]) {
    for (uint8_t i = 0; i < numDrops; i++) {
        Raindrop* drop = &raindrops[i];
        
        drop->x += WIND_DRIFT;
        drop->y += drop->speedY;
        
        if (drop->x > GFX_LCD_WIDTH + drop->length) {
            drop->x = -drop->length;
        }
        
        if (drop->y > GFX_LCD_HEIGHT + drop->length) {
            rain_initDrop(drop, true);
        }
    }
    
    return !kb_AnyKey();
}

static void rain_draw(const uint8_t numDrops, Raindrop raindrops[numDrops]) {
    gfx_ZeroScreen();

    for (uint8_t i = 0; i < numDrops; i++) {
        Raindrop *drop = &raindrops[i];

        int16_t x = (int16_t)drop->x;
        int16_t y = (int16_t)drop->y;
        uint8_t length = drop->length;
        
        uint8_t colorIndex = (length > NUM_SIZES) ? NUM_SIZES : length;
        gfx_SetColor(colorIndex);
        
        int16_t x2 = x + (int16_t)(length * drop->drift);
        gfx_Line(x, y, x2, y + length);
    }
}

int main(void) {
    gfx_Begin();
    gfx_SetDrawBuffer();
    
    for (uint16_t i = 1; i <= NUM_SIZES; i++) {
        uint8_t blue = 120 + (i * 135) / NUM_SIZES;
        uint8_t green = 80 + (i * 100) / NUM_SIZES;
        gfx_palette[i] = gfx_RGBTo1555(green, green + 20, blue);
    }
    
    Raindrop raindrops[MAX_RAINDROPS];
    rain_init(MAX_RAINDROPS, raindrops);
    
    while (rain_update(MAX_RAINDROPS, raindrops)) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }
        
        rain_draw(MAX_RAINDROPS, raindrops);
        gfx_SwapDraw();
    }
    
    gfx_End();
    return 0;
}
