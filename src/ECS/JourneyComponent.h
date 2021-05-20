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

#ifndef __JOURNEY_COMPONENT_H__
#define __JOURNEY_COMPONENT_H__

typedef struct JourneyComponent JourneyComponent;
typedef JourneyComponent * JourneyComponentPtr;

#include "ECS.h"
#include "MappedComponent.h"
#include "MoveComponent.h"
#include "../objects/room.h"

typedef enum {
	NO_DIR,
	FORWARDS_DIR,
	BACKWARDS_DIR,
	LEFT_DIR,
	RIGHT_DIR
} directions;

struct JourneyComponent {
	Component base_component;
	MappedComponentPtr mapped;
	MoveComponentPtr move;
	TilePtr target_tile;
	double target_x;
	double target_y;
	RoomPtr room;
	int path_length;
	TilePtr * path_tiles;
	PlanePtr * path_planes;
	size_t direction;
	EntityPtr end_interact;
};

ComponentPtr JourneyComponent_init(
	void * me_void, EntityPtr entity, va_list * args
);
void JourneyComponent_update(void * me_void);
void JourneyComponent_draw(void * me_void);
void JourneyComponent_delete(void * me_void);

void JourneyComponent_journey_to(
	JourneyComponentPtr me, TilePtr tile, double x, double y
);
void JourneyComponent_dijkstra(JourneyComponentPtr me);
void JourneyComponent_set_end_direction(
	JourneyComponentPtr me, size_t direction
);
void JourneyComponent_set_end_interact(
	JourneyComponentPtr me, EntityPtr end_interact
);

directions get_direction(char * str_direction);

#endif
