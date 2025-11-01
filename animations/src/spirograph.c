#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <time.h>
#include <math.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "palette.h"
#include "turtle/turtle.h"

#define PI 3.1415f
#define NUM_TURTLES 15

#define DEG2RAD(x) ((x) * (PI / 180.0f))

static float angle = 9;
static int speed = 6;
typedef enum {
    MODE_CIRCLE,
    MODE_RADIAL,
    MODE_SPIRAL,
    MODE_RANDOM,
    MODE_ORBIT,
    MODE_WAVE,
    
    MODE_COUNT
} SpiroMode;

static SpiroMode mode = MODE_CIRCLE;

void spiro_init(const uint8_t numTurtles, Turtle turtles[numTurtles]) {
    float cx = GFX_LCD_WIDTH / 2.0f;
    float cy = GFX_LCD_HEIGHT / 2.0f;
    float radius = (GFX_LCD_HEIGHT < GFX_LCD_WIDTH ? GFX_LCD_HEIGHT : GFX_LCD_WIDTH) / 3.0f;

    for (uint8_t i = 0; i < numTurtles; i++) {
        Turtle* t = &turtles[i];
        Turtle_Init(t);
        Turtle_PenUp(t);
        Turtle_SetColor(t, 255.0f / numTurtles * i + 1);

        switch (mode) {
            case MODE_CIRCLE: {
                float a = DEG2RAD(360.0f / numTurtles * i);
                Turtle_Goto(t, cx + cosf(a) * radius, cy + sinf(a) * radius);
                Turtle_SetAngle(t, (360.0f / numTurtles * i) + 90.0f);
                break;
            }
            case MODE_RADIAL: {
                Turtle_Goto(t, cx, cy);
                Turtle_SetAngle(t, 360.0f / numTurtles * i);
                break;
            }
            case MODE_SPIRAL: {
                float a = DEG2RAD(30.0f * i);
                float r = 10.0f * i;
                Turtle_Goto(t, cx + cosf(a) * r, cy + sinf(a) * r);
                Turtle_SetAngle(t, (360.0f / numTurtles * i) + 90.0f);
                break;
            }
            case MODE_RANDOM: {
                Turtle_Goto(t, rand() % GFX_LCD_WIDTH, rand() % GFX_LCD_HEIGHT);
                Turtle_SetAngle(t, rand() % 360);
                break;
            }
            case MODE_ORBIT: {
                float ring = (i % 3 + 1) * (radius / 4.0f);
                float a = DEG2RAD((360.0f / numTurtles) * i);
                Turtle_Goto(t, cx + cosf(a) * ring, cy + sinf(a) * ring);
                Turtle_SetAngle(t, (360.0f / numTurtles * i));
                break;
            }
            case MODE_WAVE: {
                float spacing = GFX_LCD_WIDTH / (float)(numTurtles + 1);
                float x = spacing * (i + 1);
                float y = cy + sinf(i * 0.5f) * (radius / 2.0f);
                Turtle_Goto(t, x, y);
                Turtle_SetAngle(t, 90.0f);
                break;
            }
            case MODE_COUNT:
                break;
        }

        Turtle_SetWrap(t, 0);
        Turtle_PenDown(t);
    }

    mode++;
    if (mode == MODE_COUNT)
        mode = MODE_CIRCLE;

    angle = (rand() % 13) + 3;
    if (rand() % 2)
        angle = -angle;

    speed = (rand() % 9) + 2;
}

void spiro_spiro(const uint8_t numTurtles, Turtle turtles[numTurtles]) {
    for (uint8_t i = 0; i < numTurtles; i++) {
        Turtle* t = &turtles[i];

        Turtle_Left(t, angle);
        Turtle_Forward(t, speed);
        Turtle_SetColor(t, (t->color + 1) % 255 + 1);

        if (t->x >= 0 && t->x < GFX_LCD_WIDTH &&
            t->y >= 0 && t->y < GFX_LCD_HEIGHT)
        {
            gfx_SetColor(t->color);
            gfx_SetPixel(t->x, t->y);
        }
    }
}


void spirograph(void) {
    gfx_Begin();
    gfx_FillScreen(0);

    gfx_palette[0] = 0;
    palette_rainbow(gfx_palette);

    srand(rtc_Time());
    
    Turtle turtles[NUM_TURTLES];
    gfx_SetDrawScreen();
    
    uint8_t counter = 0;
    while (!kb_AnyKey()) {
        if (counter == 0)
        {
            counter = 150;
            gfx_FillScreen(0);
            spiro_init(NUM_TURTLES, turtles);
        }
        spiro_spiro(NUM_TURTLES, turtles);
        palette_shift(gfx_palette);
        counter--;
    }

    gfx_End();
}
