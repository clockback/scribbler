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

#ifndef __ENTITYGETTER_H__
#define __ENTITYGETTER_H__

#include <stdarg.h>
#include <stddef.h>

typedef struct EntityGetter EntityGetter;
typedef EntityGetter * EntityGetterPtr;

typedef enum {
	RAW_ENTITY,
	ENTITY_WITH_NAME,
	MAX_ENTITY_GETTERS
} EntityGetterType;

#include "../ECS/ECS.h"

extern size_t entity_getter_sizes[MAX_ENTITY_GETTERS];

struct EntityGetter {
	EntityGetterType type;
	void * particulars;
};

void EntityGetter_init(
	EntityGetterPtr me, IoObject * base, IoHandler * io_handler, GamePtr game
);
EntityPtr EntityGetter_evaluate(EntityGetterPtr me);

void pre_init_entity_getter_functions();
void pre_init_entity_getter_sizes();
void pre_init_entity_getter_evaluate();
void init_entity_getters();

#include "entity_getters/RawEntityEntityGetter.h"
#include "entity_getters/EntityWithNameEntityGetter.h"

extern void (*init_for_entity_getter_functions[MAX_ENTITY_GETTERS]) (
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);

extern EntityPtr (*evaulate_for_entity_getter_functions[MAX_ENTITY_GETTERS]) (
	void * me_void
);

#endif
