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

#include <stdarg.h>
#include <stdio.h>

#include "SpriteComponent.h"
#include "ECS.h"

ComponentPtr SpriteComponent_init(
	void * me_void, EntityPtr entity, va_list * args
) {
	SpriteComponentPtr me = (SpriteComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, SPRITE_COMPONENT);

	me->image = Screen_load_sprite(
		me_component->entity->system->screen, va_arg(*args, const char *),
		&me->width, &me->height
	);

	me->scale = 1;
	me->disable_scaling = true;

	me->src_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	me->src_rect->x = 0;
	me->src_rect->y = 0;
	me->src_rect->w = me->width;
	me->src_rect->h = me->height;

	me->mapped = (MappedComponentPtr)Entity_fetch_component(
		entity, MAPPED_COMPONENT
	);

	me->dest_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	me->dest_rect->x = me->mapped->x;
	me->dest_rect->y = me->mapped->y;
	me->dest_rect->w = me->width;
	me->dest_rect->h = me->height;

	return me_component;
}

void SpriteComponent_update(void * me_void) {
}

void SpriteComponent_draw(void * me_void) {
	SpriteComponentPtr me = (SpriteComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;

	if (
		!me->disable_scaling && me->mapped != NULL && me->mapped->plane != NULL
	) {
		SpriteComponent_scale(me, pow(
			1 - me->mapped->plane->room->scale_rate, me->mapped->y
		));
	}
	else if (me->disable_scaling) {
		me->scale = 1;
	}

	if (me->mapped == NULL) {
		me->dest_rect->x = me->mapped->x;
		me->dest_rect->y = me->mapped->y;
	}
	else {
		ScreenPtr screen = me_component->entity->screen;
		int layer_x = Room_get_point_x(
			me->mapped->plane->room, screen, me->mapped->x, me->mapped->y
		);
		int layer_y = Room_get_point_y(
			me->mapped->plane->room, screen, me->mapped->x, me->mapped->y,
			Plane_get_height(me->mapped->plane, me->mapped->x, me->mapped->y)
		);

		me->dest_rect->w = me->width * me->scale;
		me->dest_rect->h = me->height * me->scale;
		me->dest_rect->x = (int)(layer_x - me->dest_rect->w / 2);
		me->dest_rect->y = (int)(layer_y - me->dest_rect->h);
	}

	SDL_RenderCopy(
		me_component->entity->screen->rend, me->image, me->src_rect,
		me->dest_rect
	);
}

void SpriteComponent_delete(void * me_void) {
}

void SpriteComponent_scale(SpriteComponentPtr me, double scale) {
	me->scale = scale;
	me->disable_scaling = false;
}

void SpriteComponent_enable_scaling(SpriteComponentPtr me, bool scale) {
	if (!scale) {
		me->scale = 1.0f;
	}
	me->disable_scaling = !scale;
}
