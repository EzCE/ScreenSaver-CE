#include <graphx.h>
#include <keypadc.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "utility.h"
#include "palette.h"
#include "turtle/turtle.h"

#define NUM_TURTLES 15
#define ANGLE 360
#define SPEED 10

void walk_init(const uint8_t numTurtles, Turtle turtles[numTurtles]) {
    for (uint8_t i = 0; i < numTurtles; i++) {
        Turtle* t = &turtles[i];
        Turtle_Init(t);
        Turtle_PenUp(t);

        Turtle_SetColor(t, random() % 255 + 1);
        Turtle_Goto(t, random() % GFX_LCD_WIDTH, random() % GFX_LCD_HEIGHT);
        Turtle_SetAngle(t, random());
        
        Turtle_SetWrap(t, 1);
        Turtle_PenDown(t);
    }
}

void walk_walk(const uint8_t numTurtles, Turtle turtles[numTurtles]) {
    for (uint8_t i = 0; i < numTurtles; i++) {
        Turtle* t = &turtles[i];
        Turtle_Left(t, random() % ANGLE);
        Turtle_Forward(t, random() % SPEED + SPEED);

        if (t->x >= 0 && t->x < GFX_LCD_WIDTH
            && t->y >= 0 && t->y < GFX_LCD_HEIGHT)
        {
            gfx_SetColor(t->color);
            gfx_SetPixel(t->x, t->y);
        }
    }
}

bool walk(void) {
    gfx_Begin();
    gfx_FillScreen(0);

    gfx_palette[0] = 0;
    palette_rainbow(gfx_palette);

    srand(rtc_Time());
    
    Turtle turtles[NUM_TURTLES];
    walk_init(NUM_TURTLES, turtles);
    gfx_SetDrawScreen();
    
    uint8_t counter = 0;
    while (!kb_AnyKey()) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }

        walk_walk(NUM_TURTLES, turtles);
        palette_shift(gfx_palette);
        counter++;
        if (counter == 0)
            gfx_FillScreen(0);
    }

    gfx_End();
    return false;
}
