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

#include <stdarg.h>

#include "AnimateComponent.h"
#include "ECS.h"

ComponentPtr AnimateComponent_init(
	void * me_void, EntityPtr entity, va_list * args
) {
	AnimateComponentPtr me = (AnimateComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, ANIMATE_COMPONENT);

	me->mapped = (MappedComponentPtr)Entity_fetch_component(
		entity, MAPPED_COMPONENT
	);

	me->directory = va_arg(*args, const char *);
	me->current = STAND_FORWARDS;
	me->display = true;
	me->i = 0;
	me->count = 0;
	me->scale = 1;
	me->disable_scaling = false;

	return me_component;
}

void AnimateComponent_update(void * me_void) {
	AnimateComponentPtr me = (AnimateComponentPtr)me_void;
	EntityPtr entity = ((ComponentPtr)me)->entity;

	if (Entity_has_component(entity, MOVE_COMPONENT)) {
		MoveComponentPtr move = Entity_fetch_component(entity, MOVE_COMPONENT);

		if (move->vx == 0 && move->vy == 0) {
			if (move->old_vx < 0) {
				AnimateComponent_use_sprite(me, STAND_LEFT);
			}
			else if (move->old_vx > 0) {
				AnimateComponent_use_sprite(me, STAND_RIGHT);
			}
			else if (move->old_vy < 0) {
				AnimateComponent_use_sprite(me, STAND_FORWARDS);
			}
			else if (move->old_vy > 0) {
				AnimateComponent_use_sprite(me, STAND_BACKWARDS);
			}
			return;
		}

		me->i = (me->i + 1) % 8;
		if (me->i > 0) {
			return;
		}

		size_t animation;

		if (fabs(move->vy) >= fabs(move->vx)) {
			if (move->vy > 0) {
				switch (me->count) {
				case 0:
					animation = WALK_BACKWARDS_1;
					break;
				case 1:
					animation = WALK_BACKWARDS_2;
					break;
				case 2:
					animation = WALK_BACKWARDS_3;
					break;
				case 3:
					animation = WALK_BACKWARDS_4;
					break;
				case 4:
					animation = WALK_BACKWARDS_5;
					break;
				case 5:
					animation = WALK_BACKWARDS_6;
					break;
				}
			}
			else {
				switch (me->count) {
				case 0:
					animation = WALK_FORWARDS_1;
					break;
				case 1:
					animation = WALK_FORWARDS_2;
					break;
				case 2:
					animation = WALK_FORWARDS_3;
					break;
				case 3:
					animation = WALK_FORWARDS_4;
					break;
				case 4:
					animation = WALK_FORWARDS_5;
					break;
				case 5:
					animation = WALK_FORWARDS_6;
					break;
				}
			}
		}
		else {
			if (move->vx > 0) {
				switch (me->count) {
				case 0:
					animation = WALK_RIGHT_1;
					break;
				case 1:
					animation = WALK_RIGHT_2;
					break;
				case 2:
					animation = WALK_RIGHT_3;
					break;
				case 3:
					animation = WALK_RIGHT_4;
					break;
				case 4:
					animation = WALK_RIGHT_5;
					break;
				case 5:
					animation = WALK_RIGHT_6;
					break;
				}
			}
			else {

				switch (me->count) {
				case 0:
					animation = WALK_LEFT_1;
					break;
				case 1:
					animation = WALK_LEFT_2;
					break;
				case 2:
					animation = WALK_LEFT_3;
					break;
				case 3:
					animation = WALK_LEFT_4;
					break;
				case 4:
					animation = WALK_LEFT_5;
					break;
				case 5:
					animation = WALK_LEFT_6;
					break;
				}
			}
		}

		me->count = (me->count + 1) % 6;
		AnimateComponent_use_sprite(me, animation);
	}
}

void AnimateComponent_draw(void * me_void) {
	AnimateComponentPtr me = (AnimateComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;

	if (!me->display) {
		return;
	}

	if (
		!me->disable_scaling && me->mapped != NULL && me->mapped->plane != NULL
	) {
		AnimateComponent_scale(me, pow(
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

		me->dest_rect->x = (int)(layer_x - me->dest_rect->w / 2);
		me->dest_rect->y = (int)(layer_y - me->dest_rect->h);
	}

	SDL_RenderCopy(
		me_component->entity->screen->rend, me->images[me->current],
		me->source_rects[me->current], me->dest_rect
	);
}

void AnimateComponent_delete(void * me_void) {
}

void AnimateComponent_load_sprite(
	AnimateComponentPtr me, size_t animation, char * filename
) {
	char full_filename[100];
	strcpy(full_filename, me->directory);
	strcat(full_filename, "/");
	strcat(full_filename, filename);

	int width, height;
	me->images[animation] = Screen_load_sprite(
		((ComponentPtr)me)->entity->system->screen, full_filename, &width,
		&height
	);

	me->source_rects[animation] = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	me->source_rects[animation]->x = 0;
	me->source_rects[animation]->y = 0;
	me->source_rects[animation]->w = width;
	me->source_rects[animation]->h = height;

	me->dest_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
}

void AnimateComponent_use_sprite(AnimateComponentPtr me, size_t animation) {
	me->current = animation;
	me->dest_rect->w = me->source_rects[animation]->w * me->scale;
	me->dest_rect->h = me->source_rects[animation]->h * me->scale;
}

void AnimateComponent_set_display(AnimateComponentPtr me, bool display) {
	me->display = display;
}

void AnimateComponent_scale(AnimateComponentPtr me, double scale) {
	me->scale = scale;
	me->disable_scaling = false;
	me->dest_rect->w = me->source_rects[me->current]->w * me->scale;
	me->dest_rect->h = me->source_rects[me->current]->h * me->scale;
}

void AnimateComponent_enable_scaling(AnimateComponentPtr me, bool scale) {
	if (!scale) {
		me->scale = 1.0f;
	}
	me->disable_scaling = !scale;
}

animation_frames get_animation_key(char * str_key) {
	if (strcmp(str_key, "STAND_FORWARDS") == 0) {
		return STAND_FORWARDS;
	}
	else if (strcmp(str_key, "STAND_BACKWARDS") == 0) {
		return STAND_BACKWARDS;
	}
	else if (strcmp(str_key, "STAND_LEFT") == 0) {
		return STAND_LEFT;
	}
	else if (strcmp(str_key, "STAND_RIGHT") == 0) {
		return STAND_RIGHT;
	}
	else if (strcmp(str_key, "WALK_FORWARDS_1") == 0) {
		return WALK_FORWARDS_1;
	}
	else if (strcmp(str_key, "WALK_FORWARDS_2") == 0) {
		return WALK_FORWARDS_2;
	}
	else if (strcmp(str_key, "WALK_FORWARDS_3") == 0) {
		return WALK_FORWARDS_3;
	}
	else if (strcmp(str_key, "WALK_FORWARDS_4") == 0) {
		return WALK_FORWARDS_4;
	}
	else if (strcmp(str_key, "WALK_FORWARDS_5") == 0) {
		return WALK_FORWARDS_5;
	}
	else if (strcmp(str_key, "WALK_FORWARDS_6") == 0) {
		return WALK_FORWARDS_6;
	}
	else if (strcmp(str_key, "WALK_BACKWARDS_1") == 0) {
		return WALK_BACKWARDS_1;
	}
	else if (strcmp(str_key, "WALK_BACKWARDS_2") == 0) {
		return WALK_BACKWARDS_2;
	}
	else if (strcmp(str_key, "WALK_BACKWARDS_3") == 0) {
		return WALK_BACKWARDS_3;
	}
	else if (strcmp(str_key, "WALK_BACKWARDS_4") == 0) {
		return WALK_BACKWARDS_4;
	}
	else if (strcmp(str_key, "WALK_BACKWARDS_5") == 0) {
		return WALK_BACKWARDS_5;
	}
	else if (strcmp(str_key, "WALK_BACKWARDS_6") == 0) {
		return WALK_BACKWARDS_6;
	}
	else if (strcmp(str_key, "WALK_LEFT_1") == 0) {
		return WALK_LEFT_1;
	}
	else if (strcmp(str_key, "WALK_LEFT_2") == 0) {
		return WALK_LEFT_2;
	}
	else if (strcmp(str_key, "WALK_LEFT_3") == 0) {
		return WALK_LEFT_3;
	}
	else if (strcmp(str_key, "WALK_LEFT_4") == 0) {
		return WALK_LEFT_4;
	}
	else if (strcmp(str_key, "WALK_LEFT_5") == 0) {
		return WALK_LEFT_5;
	}
	else if (strcmp(str_key, "WALK_LEFT_6") == 0) {
		return WALK_LEFT_6;
	}
	else if (strcmp(str_key, "WALK_RIGHT_1") == 0) {
		return WALK_RIGHT_1;
	}
	else if (strcmp(str_key, "WALK_RIGHT_2") == 0) {
		return WALK_RIGHT_2;
	}
	else if (strcmp(str_key, "WALK_RIGHT_3") == 0) {
		return WALK_RIGHT_3;
	}
	else if (strcmp(str_key, "WALK_RIGHT_4") == 0) {
		return WALK_RIGHT_4;
	}
	else if (strcmp(str_key, "WALK_RIGHT_5") == 0) {
		return WALK_RIGHT_5;
	}
	else if (strcmp(str_key, "WALK_RIGHT_6") == 0) {
		return WALK_RIGHT_6;
	}
	else {
		printf(
			"Error: does not recognize animation token '%s'\n", str_key
		);
		exit(-1);
	}
}
