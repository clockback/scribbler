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

#include "MoveComponent.h"
#include "MappedComponent.h"
#include "JourneyComponent.h"

ComponentPtr MoveComponent_init(void * me_void, EntityPtr entity, va_list * args) {
	MoveComponentPtr me = (MoveComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, MOVE_COMPONENT);
	me->mapped = (MappedComponentPtr)Entity_fetch_component(
		entity, MAPPED_COMPONENT
	);
	me->vx = 0;
	me->vy = 0;
	me->old_vx = 0;
	me->old_vy = 0;
	return me_component;
}

void MoveComponent_update(void * me_void) {
	MoveComponentPtr me = (MoveComponentPtr)me_void;
	MappedComponentPtr mapped = me->mapped;
	mapped->x += me->vx;
	mapped->y += me->vy;
	if (
		mapped->x < mapped->tile->x || mapped->x > mapped->tile->x + 1
		|| mapped->y < mapped->tile->y || mapped->y > mapped->tile->y + 1
	) {
		mapped->tile = Plane_get_tile(
			mapped->plane, (int)mapped->x, (int)mapped->y
		);
		mapped->plane = mapped->tile->plane;
	}
}

void MoveComponent_draw(void * me_void) {

}

void MoveComponent_delete(void * me_void) {

}

void MoveComponent_stop_moving(MoveComponentPtr me) {
	me->old_vx = (float)me->vx;
	me->old_vy = (float)me->vy;
	me->vx = 0.0f;
	me->vy = 0.0f;

	if (Entity_has_component(((ComponentPtr)me)->entity, JOURNEY_COMPONENT)) {
		JourneyComponentPtr journey = (
			(JourneyComponentPtr)Entity_fetch_component(
				((ComponentPtr)me)->entity, JOURNEY_COMPONENT
			)
		);
		if (
			journey->direction != NO_DIR
			&& me->mapped->x == journey->target_x
			&& me->mapped->y == journey->target_y
			&& me->mapped->plane == journey->target_tile->plane
		) {
			switch (journey->direction) {
			case (BACKWARDS_DIR):
				me->old_vx = 0.0f;
				me->old_vy = 1.0f;
				break;
			case (FORWARDS_DIR):
				me->old_vx = 0.0f;
				me->old_vy = -1.0f;
				break;
			case (LEFT_DIR):
				me->old_vx = -1.0f;
				me->old_vy = 0.0f;
				break;
			case (RIGHT_DIR):
				me->old_vx = 1.0f;
				me->old_vy = 0.0f;
				break;
			}
			journey->direction = NO_DIR;
		}
	}
}
