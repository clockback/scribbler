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

#ifndef __ENTITYGETY_H__
#define __ENTITYGETY_H__

typedef struct EntityGetY EntityGetY;
typedef EntityGetY * EntityGetYPtr;

#include "../../ECS/MappedComponent.h"
#include "../EntityGetter.h"

struct EntityGetY {
    EntityGetterPtr entity;
};

typedef EntityGetY * EntityGetYPtr;

void EntityGetY_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);
double EntityGetY_evaluate(void * me);

#endif
