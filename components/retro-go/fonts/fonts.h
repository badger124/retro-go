#include "../rg_gui.h"

/**
 * This file can be edited to add fonts to retro-go.
 * To create new fonts you can use font_converter.py located in the tools folder.
 *
 * HOW TO ADD A NEW FONT (e.g. a Korean font):
 * 1. Run tools/font_converter.py, select your TTF/OTF file, set the character
 *    range (e.g. 44032-55203 for Korean Hangul 가-힣), click Save.
 *    The tool saves e.g. "NanumGothic11.c" with variable "font_NanumGothic11".
 *    The variable name is always:  font_<FontName><Size>
 *    (spaces and hyphens in the font name are replaced with underscores)
 *
 * 2. Copy the generated .c file into this folder (components/retro-go/fonts/).
 *
 * 3. Add the three lines marked "ADD" below:
 *    a) extern declaration  (step A)
 *    b) enum value          (step B, before RG_FONT_MAX)
 *    c) array entry         (step C, matching position of the enum value)
 */

extern const rg_font_t font_basic8x8;
extern const rg_font_t font_DejaVu12;
extern const rg_font_t font_DejaVu15;
extern const rg_font_t font_VeraBold11;
extern const rg_font_t font_VeraBold14;
// ADD (step A): extern const rg_font_t font_NanumGothic11;

enum {
    RG_FONT_BASIC_8,
    RG_FONT_BASIC_12,
    RG_FONT_BASIC_16,
    RG_FONT_DEJAVU_12,
    RG_FONT_DEJAVU_15,
    RG_FONT_VERA_11,
    RG_FONT_VERA_14,
    // ADD (step B): RG_FONT_NANUM_11,
    RG_FONT_MAX,
};

static const rg_font_t *fonts[RG_FONT_MAX] = {
    &font_basic8x8,
    &font_basic8x8,
    &font_basic8x8,
    &font_DejaVu12,
    &font_DejaVu15,
    &font_VeraBold11,
    &font_VeraBold14,
    // ADD (step C): &font_NanumGothic11,
};
