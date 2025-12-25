#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "utility.h"

#define MAX_SNOWFLAKES      64
#define NUM_SIZES           8

#define BASE_SPEED          0.4f
#define SPEED_VARIANCE      0.9f

#define MAX_DRIFT           0.8f

#define RAND_PERCENT()      (randInt(0, 100) / 100.0f)

typedef struct Snowflake {
    float x;
    float y;
    float speedY;
    float drift;
    uint8_t size;
} Snowflake;

void snow_initFlake(Snowflake* flake, bool startAtTop) {
    flake->size = randInt(1, NUM_SIZES+1);
    
    flake->x = (float)randInt(0, GFX_LCD_WIDTH - 1);
    
    if (startAtTop) {
        flake->y = (float)(-flake->size) - randInt(0, 20);
    } else {
        flake->y = (float)randInt(0, GFX_LCD_HEIGHT - 1);
    }
    
    float sizeMultiplier = 0.5f + (flake->size * 0.3f);
    flake->speedY = BASE_SPEED + RAND_PERCENT() * SPEED_VARIANCE;
    flake->speedY *= sizeMultiplier;
    
    flake->drift = (RAND_PERCENT() * 2.0f - 1.0f) * MAX_DRIFT;
}

void snow_init(const uint8_t numFlakes, Snowflake snowflakes[numFlakes]) {
    srand(rtc_Time());
    
    for (uint8_t i = 0; i < numFlakes; i++) {
        snow_initFlake(&snowflakes[i], false);
    }
}

bool snow_update(const uint8_t numFlakes, Snowflake snowflakes[numFlakes]) {
    for (uint8_t i = 0; i < numFlakes; i++) {
        Snowflake* flake = &snowflakes[i];
        
        flake->x += flake->drift;
        flake->y += flake->speedY;
        
        if (randInt(0, 100) < 10) {
            flake->drift += (RAND_PERCENT() - 0.5f) * 0.5f;
            
            if (flake->drift > MAX_DRIFT) 
                flake->drift = MAX_DRIFT;
            
            if (flake->drift < -MAX_DRIFT) 
                flake->drift = -MAX_DRIFT;
        }
        
        if (flake->x < -flake->size) {
            flake->x = GFX_LCD_WIDTH + flake->size;
        } else if (flake->x > GFX_LCD_WIDTH + flake->size) {
            flake->x = -flake->size;
        }
        
        if (flake->y > GFX_LCD_HEIGHT + flake->size) {
            snow_initFlake(flake, true);
        }
    }
    
    return !kb_AnyKey();
}

void snow_drawFlake(const Snowflake* flake) {
    int16_t x = (int16_t)flake->x;
    int16_t y = (int16_t)flake->y;
    uint8_t radius = flake->size;
    
    gfx_SetColor(radius);
    
    if (radius == 1) {
        gfx_SetPixel(x, y);
    } else if (radius < 4) {
        gfx_FillCircle(x, y, radius - 1);
    } else {
        uint8_t armLen = radius;
        uint8_t diagLen = (armLen * 7) / 10;
        
        gfx_VertLine(x, y - armLen, armLen * 2 + 1);
        gfx_HorizLine(x - armLen, y, armLen * 2 + 1);
        
        gfx_Line(x - diagLen, y - diagLen, x + diagLen, y + diagLen);
        gfx_Line(x - diagLen, y + diagLen, x + diagLen, y - diagLen);
        
        gfx_SetPixel(x, y);
    }
}

void snow_draw(const uint8_t numFlakes, Snowflake snowflakes[numFlakes]) {
    gfx_ZeroScreen();

    for (uint8_t size = 0; size < NUM_SIZES; size++) {
        for (uint8_t i = 0; i < numFlakes; i++) {
            if (snowflakes[i].size == size) {
                snow_drawFlake(&snowflakes[i]);
            }
        }
    }
}

bool snow(void) {
    gfx_Begin();
    gfx_SetDrawBuffer();
    
    for (uint16_t i = 1; i <= NUM_SIZES; i++) {
        uint8_t shade = 20*i + 95;
        gfx_palette[i] = gfx_RGBTo1555(shade, shade, shade);
    }
    
    Snowflake snowflakes[MAX_SNOWFLAKES];
    snow_init(MAX_SNOWFLAKES, snowflakes);
    
    while (snow_update(MAX_SNOWFLAKES, snowflakes)) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }
        
        snow_draw(MAX_SNOWFLAKES, snowflakes);
        gfx_SwapDraw();
    }
    
    gfx_End();
    return false;
}
