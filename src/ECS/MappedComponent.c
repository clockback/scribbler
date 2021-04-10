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

#include "MappedComponent.h"

ComponentPtr MappedComponent_init(
	void * me_void, EntityPtr entity, va_list * args
) {
	MappedComponentPtr me = (MappedComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, MAPPED_COMPONENT);

	me->x = va_arg(*args, double);
	me->y = va_arg(*args, double);

	me->plane = va_arg(*args, PlanePtr);
	me->tile = Plane_get_tile(me->plane, (int)me->x, (int)me->y);

	return me_component;
}

void MappedComponent_update(void * me_void) {
}

void MappedComponent_draw(void * me_void) {
}

void MappedComponent_delete(void * me_void) {
}
