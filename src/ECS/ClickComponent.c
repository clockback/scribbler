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

#include "ClickComponent.h"
#include "MappedComponent.h"
#include "SpriteComponent.h"
#include "AnimateComponent.h"
#include "JourneyComponent.h"
#include "InteractComponent.h"
#include "../events/ScenarioManager.h"
#include "ECS.h"

ComponentPtr ClickComponent_init(
	void * me_void, EntityPtr entity, va_list * args
) {
	ClickComponentPtr me = (ClickComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, CLICK_COMPONENT);

	ClickComponent_set_boundary_box(me);

	me->interact_tile = NULL;
	me->interact_x = 0;
	me->interact_y = 0;
	me->interact_dir = NO_DIR;

	return me_component;
}

void ClickComponent_update(void * me_void) {
	ClickComponentPtr me = (ClickComponentPtr)me_void;
	ClickComponent_set_boundary_box(me);
}

void ClickComponent_draw(void * me_void) {
}

void ClickComponent_delete(void * me_void) {
}

void ClickComponent_set_boundary_box(ClickComponentPtr me) {
	ComponentPtr me_component = (ComponentPtr)me;
	EntityPtr entity = me_component->entity;
	ScreenPtr screen = entity->system->screen;

	MappedComponentPtr mapped = (MappedComponentPtr)Entity_fetch_component(
		entity, MAPPED_COMPONENT
	);
	if (mapped->plane == NULL) {
		me->left_boundary = mapped->x;
		me->top_boundary = mapped->y;
	}
	else {
		me->left_boundary = Room_get_point_x(
			mapped->plane->room, screen, mapped->x, mapped->y
		);
		me->top_boundary = Room_get_point_y(
			mapped->plane->room, screen, mapped->x, mapped->y,
			Plane_get_height(mapped->plane, mapped->x, mapped->y)
		);
	}

	SDL_Rect * dest;
	if (Entity_has_component(entity, SPRITE_COMPONENT)) {
		dest = ((SpriteComponentPtr)Entity_fetch_component(
			entity, SPRITE_COMPONENT
		))->dest_rect;
	}
	else if (Entity_has_component(entity, ANIMATE_COMPONENT)) {
		dest = ((SpriteComponentPtr)Entity_fetch_component(
			entity, ANIMATE_COMPONENT
		))->dest_rect;
	}
	else {
		/* This should be updated so that a custom boundary box can be set. */
		printf("Error: entity '%s' has no sprite or animation!\n", entity->id);
		exit(-1);
	}

	me->left_boundary -= dest->w / 2;
	me->top_boundary -= dest->h;
	me->right_boundary = me->left_boundary + dest->w - 1;
	me->bottom_boundary = me->top_boundary + dest->h - 1;
}

bool ClickComponent_targeting(ClickComponentPtr me, int mouse_x, int mouse_y) {
	bool between = (
		mouse_x >= me->left_boundary && mouse_x <= me->right_boundary
		&& mouse_y >= me->top_boundary && mouse_y <= me->bottom_boundary
	);

	return between;
}

void ClickComponent_set_interact_point(
	ClickComponentPtr me, TilePtr tile, double x, double y, size_t direction
) {
	me->interact_tile = tile;
	me->interact_x = x;
	me->interact_y = y;
	me->interact_dir = direction;
}

void ClickComponent_journey_to(
	ClickComponentPtr me, int mouse_x, RoomPtr room
) {
	EntityPtr entity = ((ComponentPtr)me)->entity;
	MappedComponentPtr mapped = ((MappedComponentPtr)Entity_fetch_component(
		entity, MAPPED_COMPONENT
	));

	EntityPtr * players = System_get_group(entity->system, GROUP_PLAYERS);
	size_t no_players = System_get_group_size(entity->system, GROUP_PLAYERS);

	double x;
	double y;
	TilePtr tile;
	if (me->interact_tile == NULL) {
		x = Plane_get_mapped_x(
			mapped->plane, entity->system->screen, mouse_x, mapped->y
		);
		y = (double)(int)mapped->y;
		if (mapped->y - 1 < mapped->plane->min_x) {
			printf(
				"Error! Entity '%s' has Click component, but the default "
				"interaction point is inaccessible.\n", entity->id
			);
			exit(-1);
		}
		tile = Plane_get_tile(
			mapped->plane, (int)x, (int)mapped->y - 1
		);
	}
	else {
		x = me->interact_x;
		y = me->interact_y;
		tile = me->interact_tile;
	}

	InteractComponentPtr interact = (
		(InteractComponentPtr)Entity_fetch_component(entity, INTERACT_COMPONENT)
	);

	for (size_t i = 0; i < no_players; i ++) {
		JourneyComponentPtr journey = Entity_fetch_component(
			players[i], JOURNEY_COMPONENT
		);
		JourneyComponent_journey_to(journey, tile, x, y);
		JourneyComponent_set_end_direction(journey, me->interact_dir);

		if (interact != NULL) {
			JourneyComponent_set_end_interact(journey, entity);
		}
	}
}
