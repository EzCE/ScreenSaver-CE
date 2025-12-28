#include "utility.h"

#include "gfx/gfx.h"

#include <graphx.h>
#include <keypadc.h>
#include <string.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

static void setPipeColor(uint8_t color) {
    for (uint8_t i = 0; i < straight_width * straight_height; i++) {
        if (straight->data[i]) {
            straight->data[i] = color;
        }

        if (corner1->data[i]) {
            corner1->data[i] = color;
        }

        if (corner2->data[i]) {
            corner2->data[i] = color;
        }
    }
}

static void drawSegment(unsigned int x, uint8_t y, uint8_t direction, uint8_t newDirection) {
    static uint8_t tmpdat[straight_size] = {
        5, 5,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    if (direction == newDirection) {
        if (direction < LEFT) {
            memcpy(tmpdat, straight, straight_size);
        } else {
            gfx_RotateSpriteC(straight, (gfx_sprite_t *)tmpdat);
        }
    } else {
        if ((direction & 1) == (newDirection & 1)) {
            if (direction == DOWN || newDirection == UP) {
                memcpy(tmpdat, corner1, corner1_size);
            } else {
                memcpy(tmpdat, corner2, corner2_size);
            }
        } else {
            if (direction == DOWN || newDirection == UP) {
                gfx_FlipSpriteX(corner2, (gfx_sprite_t *)tmpdat);
            } else {
                gfx_FlipSpriteX(corner1, (gfx_sprite_t *)tmpdat);
            }
        }
    }

    gfx_Sprite((gfx_sprite_t *)tmpdat, x, y);
}

int main(void) {
    gfx_Begin();
    gfx_SetPalette(palette_pipes, sizeof_palette_pipes, 0);
    gfx_ZeroScreen();
    srand(rtc_Time());

    unsigned int x = randInt(0, 319);
    uint8_t y = 0;
    uint8_t direction = DOWN;
    uint8_t newDirection = DOWN;

    setPipeColor(randInt(1, 7));

    while (!kb_AnyKey()) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }

        if (randInt(0, 4)) {
            newDirection = direction;
        } else {
            newDirection  = randInt(UP, RIGHT);
        }

        while (newDirection == (direction ^ 1)) {
            newDirection = randInt(UP, RIGHT);
        }

        drawSegment(x, y, direction, newDirection);
        direction = newDirection;

        switch (direction) {
            case UP:
                if (y) {
                    y -= 5;
                } else {
                    y = 239;
                    setPipeColor(randInt(1, 7));
                }

                break;
            case DOWN:
                if (y < 239) {
                    y += 5;
                } else {
                    y = 0;
                    setPipeColor(randInt(1, 7));
                }

                break;
            case LEFT:
                if (x) {
                    x -= 5;
                } else {
                    x = 319;
                    setPipeColor(randInt(1, 7));
                }

                break;
            case RIGHT:
                if (x < 319) {
                    x += 5;
                } else {
                    x = 0;
                    setPipeColor(randInt(1, 7));
                }

                break;
            default:
                break;
        }

        for (clock_t offset = clock(); clock() - offset < CLOCKS_PER_SEC / 28 && !kb_AnyKey(););
    }

    gfx_End();

    return 0;
}
