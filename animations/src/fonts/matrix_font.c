#include "matrix_font.h"

/* This contains the raw data for the font. */
static const uint8_t matrix_font_data[] = {
    #include "matrix_font.inc"
};
/* However, C89 does not allow us to typecast a byte array into a
fontlib_font_t pointer directly, so we have to use a second statement to do it,
though helpfully we can at least do it in the global scope. */
const fontlib_font_t *matrix_font = (fontlib_font_t *)matrix_font_data;