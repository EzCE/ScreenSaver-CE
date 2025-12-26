#include <graphx.h>
#include <keypadc.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "utility.h"
#include "palette.h"

#define NUM_BUBBLES 30
#define MIN_RADIUS 10
#define MAX_RADIUS 20

typedef struct Bubble {
    int16_t x;
    int16_t y;
    int16_t dx;
    int16_t dy;
    uint8_t radius;
    uint8_t color;
} Bubble;

static void bubble_init(const uint8_t numBubbles, Bubble bubbles[numBubbles]) {
    uint8_t cols = 5;
    uint8_t rows = (numBubbles + cols - 1) / cols;
    
    int16_t cellWidth = GFX_LCD_WIDTH / cols;
    int16_t cellHeight = GFX_LCD_HEIGHT / rows;
    
    for (uint8_t i = 0; i < numBubbles; i++) {
        Bubble *bubble = &bubbles[i];
        bubble->radius = (random() % (MAX_RADIUS - MIN_RADIUS + 1)) + MIN_RADIUS;
        
        uint8_t col = i % cols;
        uint8_t row = i / cols;
        
        bubble->x = col * cellWidth + cellWidth / 2;
        bubble->y = row * cellHeight + cellHeight / 2;

        bubble->dx = (random() % 3) + 1;
        bubble->dy = (random() % 3) + 1;

        if (random() % 2) 
            bubble->dx = -bubble->dx;
        if (random() % 2) 
            bubble->dy = -bubble->dy;
        
        bubble->color = (random() % 255) + 1;
    }
}

static bool bubble_update(const uint8_t numBubbles, Bubble bubbles[numBubbles]) {
    for (uint8_t i = 0; i < numBubbles; i++) {
        Bubble *bubble = &bubbles[i];

        bubble->x += bubble->dx;
        bubble->y += bubble->dy;

        if (bubble->x - bubble->radius <= 0) {
            bubble->x = bubble->radius + 1;
            bubble->dx = -bubble->dx;
        } else if (bubble->x + bubble->radius + 2 >= GFX_LCD_WIDTH) {
            bubble->x = GFX_LCD_WIDTH - bubble->radius - 2;
            bubble->dx = -bubble->dx;
        }

        if (bubble->y - bubble->radius <= 0) {
            bubble->y = bubble->radius + 1;
            bubble->dy = -bubble->dy;
        } else if (bubble->y + bubble->radius + 2 >= GFX_LCD_HEIGHT) {
            bubble->y = GFX_LCD_HEIGHT - bubble->radius - 2;
            bubble->dy = -bubble->dy;
        }
    }

    for (uint8_t i = 0; i < numBubbles - 1; i++) {
        Bubble *a = &bubbles[i];
        for (uint8_t j = i + 1; j < numBubbles; j++) {
            Bubble *b = &bubbles[j];
            
            int16_t dx = b->x - a->x;
            int16_t dy = b->y - a->y;
            int16_t minDist = a->radius + b->radius + 2;
            
            int32_t distSq = (int32_t)dx * dx + (int32_t)dy * dy;
            int32_t minDistSq = (int32_t)minDist * minDist;
            
            if (distSq < minDistSq && distSq > 0) {
                int16_t adx = dx < 0 ? -dx : dx;
                int16_t ady = dy < 0 ? -dy : dy;
                int16_t approxDist = (adx > ady) ? (adx + ady / 2) : (ady + adx / 2);
                if (approxDist == 0) 
                    approxDist = 1;
                
                int16_t overlap = minDist - approxDist;
                if (overlap > 0) {
                    int16_t pushX = (dx * (overlap / 2 + 1)) / approxDist;
                    int16_t pushY = (dy * (overlap / 2 + 1)) / approxDist;
                    a->x -= pushX;
                    a->y -= pushY;
                    b->x += pushX;
                    b->y += pushY;
                }
                
                int16_t dvx = a->dx - b->dx;
                int16_t dvy = a->dy - b->dy;
                
                int32_t dvDotN = (int32_t)dvx * dx + (int32_t)dvy * dy;
                
                if (dvDotN > 0) {
                    int16_t tempDx = a->dx;
                    int16_t tempDy = a->dy;
                    a->dx = b->dx;
                    a->dy = b->dy;
                    b->dx = tempDx;
                    b->dy = tempDy;
                }
            }
        }
    }

    return !kb_AnyKey();
}

static void bubble_draw(const uint8_t numBubbles, Bubble bubbles[numBubbles]) {
    for (uint8_t i = 0; i < numBubbles; i++) {
        Bubble *bubble = &bubbles[i];
        gfx_SetColor(bubble->color);
        gfx_Circle(bubble->x, bubble->y, bubble->radius);
    }
}

bool bubbles(void) {
    gfx_Begin();
    gfx_FillScreen(0);

    gfx_palette[0] = 0;
    palette_rainbow(gfx_palette);

    srand(rtc_Time());

    Bubble bubble_array[NUM_BUBBLES];
    bubble_init(NUM_BUBBLES, bubble_array);

    gfx_SetDrawScreen();

    while (bubble_update(NUM_BUBBLES, bubble_array)) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }
        
        gfx_ZeroScreen();
        bubble_draw(NUM_BUBBLES, bubble_array);
        palette_shift(gfx_palette);
        
        gfx_SwapDraw();
    }

    gfx_End();
    return false;
}
