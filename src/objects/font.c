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

#include "./font.h"

void Font_init(
	FontPtr me, const char * path, ScreenPtr screen, int max_width
) {
	me->no_text_blocks = 0;
	me->max_width = max_width;
	me->screen = screen;

	for (glyph i = 0; i < MAX_GLYPHS; i ++) {
		Font_load_character(me, i, path);
	}

	me->text_blocks = (TextBlockPtr *) malloc(sizeof(TextBlockPtr));
}

void Font_load_character(FontPtr me, glyph i, const char * path) {
	char full_filename[200];
	strcpy(full_filename, path);
	strcat(full_filename, "/");

	switch (i) {
    case LETTER_A_LOWER:
        strcat(full_filename, "letter_a_lower");
        break;
    case LETTER_B_LOWER:
        strcat(full_filename, "letter_b_lower");
        break;
    case LETTER_C_LOWER:
        strcat(full_filename, "letter_c_lower");
        break;
    case LETTER_D_LOWER:
        strcat(full_filename, "letter_d_lower");
        break;
    case LETTER_E_LOWER:
        strcat(full_filename, "letter_e_lower");
        break;
    case LETTER_F_LOWER:
        strcat(full_filename, "letter_f_lower");
        break;
    case LETTER_G_LOWER:
        strcat(full_filename, "letter_g_lower");
        break;
    case LETTER_H_LOWER:
        strcat(full_filename, "letter_h_lower");
        break;
    case LETTER_I_LOWER:
        strcat(full_filename, "letter_i_lower");
        break;
    case LETTER_J_LOWER:
        strcat(full_filename, "letter_j_lower");
        break;
    case LETTER_K_LOWER:
        strcat(full_filename, "letter_k_lower");
        break;
    case LETTER_L_LOWER:
        strcat(full_filename, "letter_l_lower");
        break;
    case LETTER_M_LOWER:
        strcat(full_filename, "letter_m_lower");
        break;
    case LETTER_N_LOWER:
        strcat(full_filename, "letter_n_lower");
        break;
    case LETTER_O_LOWER:
        strcat(full_filename, "letter_o_lower");
        break;
    case LETTER_P_LOWER:
        strcat(full_filename, "letter_p_lower");
        break;
    case LETTER_Q_LOWER:
        strcat(full_filename, "letter_q_lower");
        break;
    case LETTER_R_LOWER:
        strcat(full_filename, "letter_r_lower");
        break;
    case LETTER_S_LOWER:
        strcat(full_filename, "letter_s_lower");
        break;
    case LETTER_T_LOWER:
        strcat(full_filename, "letter_t_lower");
        break;
    case LETTER_U_LOWER:
        strcat(full_filename, "letter_u_lower");
        break;
    case LETTER_V_LOWER:
        strcat(full_filename, "letter_v_lower");
        break;
    case LETTER_W_LOWER:
        strcat(full_filename, "letter_w_lower");
        break;
    case LETTER_X_LOWER:
        strcat(full_filename, "letter_x_lower");
        break;
    case LETTER_Y_LOWER:
        strcat(full_filename, "letter_y_lower");
        break;
    case LETTER_Z_LOWER:
        strcat(full_filename, "letter_z_lower");
        break;
    case LETTER_A_UPPER:
        strcat(full_filename, "letter_a_upper");
        break;
    case LETTER_B_UPPER:
        strcat(full_filename, "letter_b_upper");
        break;
    case LETTER_C_UPPER:
        strcat(full_filename, "letter_c_upper");
        break;
    case LETTER_D_UPPER:
        strcat(full_filename, "letter_d_upper");
        break;
    case LETTER_E_UPPER:
        strcat(full_filename, "letter_e_upper");
        break;
    case LETTER_F_UPPER:
        strcat(full_filename, "letter_f_upper");
        break;
    case LETTER_G_UPPER:
        strcat(full_filename, "letter_g_upper");
        break;
    case LETTER_H_UPPER:
        strcat(full_filename, "letter_h_upper");
        break;
    case LETTER_I_UPPER:
        strcat(full_filename, "letter_i_upper");
        break;
    case LETTER_J_UPPER:
        strcat(full_filename, "letter_j_upper");
        break;
    case LETTER_K_UPPER:
        strcat(full_filename, "letter_k_upper");
        break;
    case LETTER_L_UPPER:
        strcat(full_filename, "letter_l_upper");
        break;
    case LETTER_M_UPPER:
        strcat(full_filename, "letter_m_upper");
        break;
    case LETTER_N_UPPER:
        strcat(full_filename, "letter_n_upper");
        break;
    case LETTER_O_UPPER:
        strcat(full_filename, "letter_o_upper");
        break;
    case LETTER_P_UPPER:
        strcat(full_filename, "letter_p_upper");
        break;
    case LETTER_Q_UPPER:
        strcat(full_filename, "letter_q_upper");
        break;
    case LETTER_R_UPPER:
        strcat(full_filename, "letter_r_upper");
        break;
    case LETTER_S_UPPER:
        strcat(full_filename, "letter_s_upper");
        break;
    case LETTER_T_UPPER:
        strcat(full_filename, "letter_t_upper");
        break;
    case LETTER_U_UPPER:
        strcat(full_filename, "letter_u_upper");
        break;
    case LETTER_V_UPPER:
        strcat(full_filename, "letter_v_upper");
        break;
    case LETTER_W_UPPER:
        strcat(full_filename, "letter_w_upper");
        break;
    case LETTER_X_UPPER:
        strcat(full_filename, "letter_x_upper");
        break;
    case LETTER_Y_UPPER:
        strcat(full_filename, "letter_y_upper");
        break;
    case LETTER_Z_UPPER:
        strcat(full_filename, "letter_z_upper");
        break;
    case WHITE_SPACE:
        strcat(full_filename, "white_space");
        break;
    case FULL_STOP:
        strcat(full_filename, "full_stop");
        break;
    case EXCLAMATION_MARK:
        strcat(full_filename, "exclamation_mark");
        break;
    case QUESTION_MARK:
        strcat(full_filename, "question_mark");
        break;
    default:
    	printf("Attempt to load unknown glyph, %d!\n", i);
    	exit(-1);
	}

	strcat(full_filename, ".png");

	me->glyph_rects[i] = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	me->glyph_rects[i]->x = 0;
	me->glyph_rects[i]->y = 0;

	me->glyph_images[i] = Screen_load_sprite(
		me->screen, full_filename, &(me->glyph_rects[i]->w),
		&(me->glyph_rects[i]->h)
	);
}

SDL_Texture * Font_render_text(
	FontPtr me, const char * text, SDL_Rect * rect
) {
	glyph glyph_text[1000];
	int total_width = 0;
	int total_height = 0;

	for (int i = 0; text[i] != '\0'; i ++) {
		glyph g;
		switch (text[i]) {
        case 'A':
            g = LETTER_A_UPPER;
            break;
        case 'B':
            g = LETTER_B_UPPER;
            break;
        case 'C':
            g = LETTER_C_UPPER;
            break;
        case 'D':
            g = LETTER_D_UPPER;
            break;
        case 'E':
            g = LETTER_E_UPPER;
            break;
        case 'F':
            g = LETTER_F_UPPER;
            break;
        case 'G':
            g = LETTER_G_UPPER;
            break;
        case 'H':
            g = LETTER_H_UPPER;
            break;
        case 'I':
            g = LETTER_I_UPPER;
            break;
        case 'J':
            g = LETTER_J_UPPER;
            break;
        case 'K':
            g = LETTER_K_UPPER;
            break;
        case 'L':
            g = LETTER_L_UPPER;
            break;
        case 'M':
            g = LETTER_M_UPPER;
            break;
        case 'N':
            g = LETTER_N_UPPER;
            break;
        case 'O':
            g = LETTER_O_UPPER;
            break;
        case 'P':
            g = LETTER_P_UPPER;
            break;
        case 'Q':
            g = LETTER_Q_UPPER;
            break;
        case 'R':
            g = LETTER_R_UPPER;
            break;
        case 'S':
            g = LETTER_S_UPPER;
            break;
        case 'T':
            g = LETTER_T_UPPER;
            break;
        case 'U':
            g = LETTER_U_UPPER;
            break;
        case 'V':
            g = LETTER_V_UPPER;
            break;
        case 'W':
            g = LETTER_W_UPPER;
            break;
        case 'X':
            g = LETTER_X_UPPER;
            break;
        case 'Y':
            g = LETTER_Y_UPPER;
            break;
        case 'Z':
            g = LETTER_Z_UPPER;
            break;
        case 'a':
            g = LETTER_A_LOWER;
            break;
        case 'b':
            g = LETTER_B_LOWER;
            break;
        case 'c':
            g = LETTER_C_LOWER;
            break;
        case 'd':
            g = LETTER_D_LOWER;
            break;
        case 'e':
            g = LETTER_E_LOWER;
            break;
        case 'f':
            g = LETTER_F_LOWER;
            break;
        case 'g':
            g = LETTER_G_LOWER;
            break;
        case 'h':
            g = LETTER_H_LOWER;
            break;
        case 'i':
            g = LETTER_I_LOWER;
            break;
        case 'j':
            g = LETTER_J_LOWER;
            break;
        case 'k':
            g = LETTER_K_LOWER;
            break;
        case 'l':
            g = LETTER_L_LOWER;
            break;
        case 'm':
            g = LETTER_M_LOWER;
            break;
        case 'n':
            g = LETTER_N_LOWER;
            break;
        case 'o':
            g = LETTER_O_LOWER;
            break;
        case 'p':
            g = LETTER_P_LOWER;
            break;
        case 'q':
            g = LETTER_Q_LOWER;
            break;
        case 'r':
            g = LETTER_R_LOWER;
            break;
        case 's':
            g = LETTER_S_LOWER;
            break;
        case 't':
            g = LETTER_T_LOWER;
            break;
        case 'u':
            g = LETTER_U_LOWER;
            break;
        case 'v':
            g = LETTER_V_LOWER;
            break;
        case 'w':
            g = LETTER_W_LOWER;
            break;
        case 'x':
            g = LETTER_X_LOWER;
            break;
        case 'y':
            g = LETTER_Y_LOWER;
            break;
        case 'z':
            g = LETTER_Z_LOWER;
            break;
        case ' ':
            g = WHITE_SPACE;
            break;
        case '.':
            g = FULL_STOP;
            break;
        case '!':
            g = EXCLAMATION_MARK;
            break;
        case '?':
            g = QUESTION_MARK;
            break;
        default:
        	printf("Attempt to render unknown glyph, %d!\n", i);
        	exit(-1);
		}
		glyph_text[i] = g;
		total_width += me->glyph_rects[g]->w;
		if (total_height < me->glyph_rects[g]->h) {
			total_height = me->glyph_rects[g]->h;
		}
	}

	SDL_Texture * image = SDL_CreateTexture(
		me->screen->rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		total_width, total_height
	);
	SDL_SetTextureBlendMode(image, SDL_BLENDMODE_BLEND);

	rect->w = total_width;
	rect->h = total_height;
	rect->x = 0;
	rect->y = 0;

    SDL_SetRenderTarget(me->screen->rend, image);

    int x = 0;

	for (int i = 0; text[i] != '\0'; i ++) {
		glyph g = glyph_text[i];
		SDL_Rect rend_pos;
		rend_pos.x = x;
		rend_pos.y = 0;
		rend_pos.w = me->glyph_rects[g]->w;
		rend_pos.h = me->glyph_rects[g]->h;
		x += rend_pos.w;

		SDL_RenderCopy(
			me->screen->rend, me->glyph_images[g], me->glyph_rects[g],
			&rend_pos
		);
	}

	return image;
}

TextBlockPtr Font_add_text(FontPtr me, const char * text) {
	me->text_blocks = (TextBlockPtr *) realloc(
		me->text_blocks, (me->no_text_blocks + 1) * sizeof(TextBlockPtr)
	);
	me->text_blocks[me->no_text_blocks] = (TextBlockPtr) malloc(
		sizeof(TextBlock)
	);
	TextBlockPtr text_block = me->text_blocks[me->no_text_blocks];

	text_block->font = me;
	text_block->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	text_block->dest_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	text_block->image = Font_render_text(me, text, text_block->rect);

	SDL_Rect * dest_rect = text_block->dest_rect;
	dest_rect->w = text_block->rect->w;
	dest_rect->h = text_block->rect->h;
	me->no_text_blocks ++;
	text_block->visible = true;

	return text_block;
}

void TextBlock_position(
	TextBlockPtr text_block, int x, int y, anchor anchor_point
) {
	SDL_Rect * dest_rect = text_block->dest_rect;
	dest_rect->x = x;
	dest_rect->y = y;

	if (anchor_point == ANCHOR_CENTRE) {
		dest_rect->x -= dest_rect->w / 2;
		dest_rect->y -= dest_rect->h / 2;
	}
	else if (anchor_point == ANCHOR_BOTTOM_CENTRE) {
		dest_rect->x -= dest_rect->w / 2;
		dest_rect->y -= dest_rect->h;
	}

	if (dest_rect->x < 0) {
		dest_rect->x = 0;
	}
	else if (dest_rect->x + dest_rect->w > text_block->font->screen->width) {
		dest_rect->x = text_block->font->screen->width - dest_rect->w;
	}

	if (dest_rect->y < 0) {
		dest_rect->y = 0;
	}
	else if (dest_rect->y + dest_rect->h > text_block->font->screen->height) {
		dest_rect->y = text_block->font->screen->height - dest_rect->y;
	}
}

void TextBlock_destroy(TextBlockPtr me) {
	FontPtr font = me->font;
	bool found = false;

	free(me->rect);
	SDL_DestroyTexture(me->image);

	for (int i = 0; i < font->no_text_blocks; i ++) {
		if (font->text_blocks[i] == me) {
			found = true;
		}
		else if (found) {
			me->font->text_blocks[i - 1] = me->font->text_blocks[i];
		}
	}

	font->no_text_blocks --;
	font->text_blocks = (TextBlockPtr *) realloc(
		font->text_blocks, font->no_text_blocks * sizeof(TextBlockPtr)
	);
}
