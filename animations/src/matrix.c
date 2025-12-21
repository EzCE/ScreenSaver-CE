#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <time.h>
#include <fontlibc.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "utility.h"
#include "fonts/matrix_font.h"

#define MATRIX_TRAIL_LENGTH 12
#define MATRIX_NUM_GLYPHS 135
#define MATRIX_GET_GLYPH() (randInt(0, MATRIX_NUM_GLYPHS-1))

typedef struct {
    uint8_t row;
    uint8_t tileIndex;
    uint8_t trail[MATRIX_TRAIL_LENGTH];
} MatrixDrop;

static void draw_matrix_glyph(uint8_t tileIndex, uint8_t paletteIndex, int16_t x, int16_t y) {
    fontlib_SetColors(paletteIndex, 0);
    fontlib_SetCursorPosition(x, y);
    fontlib_DrawGlyph(tileIndex);
}

bool matrix(void) {
    gfx_Begin();
    gfx_SetDrawBuffer();
    
    fontlib_SetFont(matrix_font, 0);
    fontlib_SetWindowFullScreen();
    srand(rtc_Time());

    const uint8_t tileWidth = 16;
    const uint8_t tileHeight = 16;

    const uint8_t cols = GFX_LCD_WIDTH / tileWidth;
    const uint8_t rows = GFX_LCD_HEIGHT / tileHeight;

    MatrixDrop* drops = malloc(cols * sizeof(MatrixDrop));
    for (uint8_t i = 0; i < cols; i++) {
        drops[i].row = randInt(0, rows);
        drops[i].tileIndex = MATRIX_GET_GLYPH();
        for (uint8_t t = 0; t < MATRIX_TRAIL_LENGTH; t++) {
            drops[i].trail[t] = MATRIX_GET_GLYPH();
        }
    }

    gfx_palette[0] = 0;

    for (uint16_t i = 1; i < 256; i++)
        gfx_palette[i] = gfx_RGBTo1555(0, 256 - i, 0);
    
    fontlib_SetColors(1, 0);
    
    while (!kb_IsDown(kb_KeyClear)) {
        if (utility_ChkAPDTimer()) {
            free(drops);
            gfx_End();
            return true;
        }
        
        gfx_ZeroScreen();

        for (uint8_t i = 0; i < cols; i++) {
            int16_t x = i * tileWidth;
            int16_t headY = (drops[i].row * tileHeight) % GFX_LCD_HEIGHT;

            for (uint8_t t = 0; t < MATRIX_TRAIL_LENGTH; t++) {
                uint8_t paletteIndex = 1 + (uint8_t)(((uint16_t)(t + 1) * 255) / (MATRIX_TRAIL_LENGTH + 1));
                int16_t trailY = (int16_t)drops[i].row * tileHeight - (int16_t)(t + 1) * tileHeight;

                while (trailY < 0) {
                    trailY += GFX_LCD_HEIGHT;
                }
                if (trailY >= GFX_LCD_HEIGHT) {
                    trailY %= GFX_LCD_HEIGHT;
                }

                draw_matrix_glyph(drops[i].trail[t], paletteIndex, x, trailY);
            }

            draw_matrix_glyph(drops[i].tileIndex, 1, x, headY);

            for (int8_t t = MATRIX_TRAIL_LENGTH - 1; t > 0; t--) {
                drops[i].trail[t] = drops[i].trail[t - 1];
            }
            drops[i].trail[0] = drops[i].tileIndex;

            drops[i].row = (drops[i].row + 1) % rows;
            drops[i].tileIndex = MATRIX_GET_GLYPH();
        }

        gfx_SwapDraw();
    }

    free(drops);
    gfx_End();

    return false;
}
