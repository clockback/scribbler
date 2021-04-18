/**
 * Scribbler
 * Copyright (C) 2021 Elliot Paton-Simpson
 *
 * This file is part of Scribbler.
 *
 *  Scribbler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Scribbler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Scribbler.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __FONT_H__
#define __FONT_H__

#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "./graphics.h"

typedef enum {
    LETTER_A_LOWER,
    LETTER_B_LOWER,
    LETTER_C_LOWER,
    LETTER_D_LOWER,
    LETTER_E_LOWER,
    LETTER_F_LOWER,
    LETTER_G_LOWER,
    LETTER_H_LOWER,
    LETTER_I_LOWER,
    LETTER_J_LOWER,
    LETTER_K_LOWER,
    LETTER_L_LOWER,
    LETTER_M_LOWER,
    LETTER_N_LOWER,
    LETTER_O_LOWER,
    LETTER_P_LOWER,
    LETTER_Q_LOWER,
    LETTER_R_LOWER,
    LETTER_S_LOWER,
    LETTER_T_LOWER,
    LETTER_U_LOWER,
    LETTER_V_LOWER,
    LETTER_W_LOWER,
    LETTER_X_LOWER,
    LETTER_Y_LOWER,
    LETTER_Z_LOWER,
    LETTER_A_UPPER,
    LETTER_B_UPPER,
    LETTER_C_UPPER,
    LETTER_D_UPPER,
    LETTER_E_UPPER,
    LETTER_F_UPPER,
    LETTER_G_UPPER,
    LETTER_H_UPPER,
    LETTER_I_UPPER,
    LETTER_J_UPPER,
    LETTER_K_UPPER,
    LETTER_L_UPPER,
    LETTER_M_UPPER,
    LETTER_N_UPPER,
    LETTER_O_UPPER,
    LETTER_P_UPPER,
    LETTER_Q_UPPER,
    LETTER_R_UPPER,
    LETTER_S_UPPER,
    LETTER_T_UPPER,
    LETTER_U_UPPER,
    LETTER_V_UPPER,
    LETTER_W_UPPER,
    LETTER_X_UPPER,
    LETTER_Y_UPPER,
    LETTER_Z_UPPER,
    WHITE_SPACE,
    FULL_STOP,
    EXCLAMATION_MARK,
    QUESTION_MARK,
    MAX_GLYPHS
} glyph;

typedef enum {
	ANCHOR_TOP_LEFT,
	ANCHOR_CENTRE,
	ANCHOR_BOTTOM_CENTRE
} anchor;

typedef struct Font Font;
typedef struct TextBlock TextBlock;

typedef TextBlock * TextBlockPtr;
typedef Font * FontPtr;

struct Font {
    SDL_Texture * glyph_images[MAX_GLYPHS];
    SDL_Rect * glyph_rects[MAX_GLYPHS];
    int max_width;
    TextBlockPtr * text_blocks;
    int no_text_blocks;
    ScreenPtr screen;
};

struct TextBlock {
	SDL_Texture * image;
	SDL_Rect * rect;
	FontPtr font;
	SDL_Rect * dest_rect;
	bool visible;
};

void Font_init(FontPtr me, const char * path, ScreenPtr screen, int max_width);
void Font_load_character(FontPtr me, glyph i, const char * path);
SDL_Texture * Font_render_text(FontPtr me, const char * text, SDL_Rect * rect);
TextBlock * Font_add_text(FontPtr me, const char * text);

void TextBlock_position(
	TextBlockPtr text_block, int x, int y, anchor anchor_point
);
void TextBlock_destroy(TextBlockPtr text_block);

#endif
