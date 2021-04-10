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

#ifndef __CLICK_COMPONENT_H__
#define __CLICK_COMPONENT_H__

#include "ECS.h"
#include "../objects/room.h"

typedef struct {
	Component base_component;
	int left_boundary;
	int right_boundary;
	int top_boundary;
	int bottom_boundary;
	TilePtr interact_tile;
	double interact_x;
	double interact_y;
	size_t interact_dir;
} ClickComponent;

typedef ClickComponent * ClickComponentPtr;

ComponentPtr ClickComponent_init(
	void * me_void, EntityPtr entity, va_list * args
);
void ClickComponent_update(void * me_void);
void ClickComponent_draw(void * me_void);
void ClickComponent_delete(void * me_void);

void ClickComponent_set_boundary_box(ClickComponentPtr me);
bool ClickComponent_targeting(ClickComponentPtr me, int mouse_x, int mouse_y);
void ClickComponent_set_interact_point(
	ClickComponentPtr me, TilePtr tile, double x, double y, size_t direction
);
void ClickComponent_journey_to(
	ClickComponentPtr me, int mouse_x, RoomPtr room
);

#endif
