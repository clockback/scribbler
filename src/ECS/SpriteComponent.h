/**
 * Scribbler
 * Copyright (C) 2022 Elliot Paton-Simpson
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

#ifndef __SPRITE_COMPONENT_H__
#define __SPRITE_COMPONENT_H__

#include "ECS.h"
#include "MappedComponent.h"

typedef struct {
	Component base_component;
	SDL_Texture * image;
	SDL_Rect * src_rect;
	SDL_Rect * dest_rect;
	MappedComponentPtr mapped;
	double scale;
	bool disable_scaling;
	int width;
	int height;
} SpriteComponent;

typedef SpriteComponent * SpriteComponentPtr;

ComponentPtr SpriteComponent_init(
	void * me_void, EntityPtr entity, va_list * args
);
void SpriteComponent_update(void * me_void);
void SpriteComponent_draw(void * me_void);
void SpriteComponent_delete(void * me_void);

void SpriteComponent_scale(SpriteComponentPtr me, double scale);
void SpriteComponent_enable_scaling(SpriteComponentPtr me, bool scale);

#endif
