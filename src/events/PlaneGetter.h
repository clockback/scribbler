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

#ifndef __PLANEGETTER_H__
#define __PLANEGETTER_H__

#include <stdarg.h>
#include <stddef.h>

typedef struct PlaneGetter PlaneGetter;
typedef PlaneGetter * PlaneGetterPtr;

typedef enum {
	RAW_PLANE,
	MAX_PLANE_GETTERS
} PlaneGetterType;

#include "../objects/room.h"
#include "../objects/IoHandler.h"

extern size_t plane_getter_sizes[MAX_PLANE_GETTERS];

struct PlaneGetter {
	PlaneGetterType type;
	void * particulars;
};

void PlaneGetter_init(
	PlaneGetterPtr me, IoObject * base, IoHandler * io_handler, GamePtr game
);
PlanePtr PlaneGetter_evaluate(PlaneGetterPtr me);

void pre_init_plane_getter_functions();
void pre_init_plane_getter_sizes();
void pre_init_plane_getter_evaluate();
void init_plane_getters();

#include "plane_getters/RawPlanePlaneGetter.h"

extern void (*init_for_plane_getter_functions[MAX_PLANE_GETTERS]) (
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);

extern PlanePtr (*evaulate_for_plane_getter_functions[MAX_PLANE_GETTERS]) (
	void * me_void
);

#endif
