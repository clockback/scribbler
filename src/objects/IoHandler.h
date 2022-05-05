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

#ifndef __IO_HANDLER_H__
#define __IO_HANDLER_H__

typedef struct IoHandler IoHandler;
typedef IoHandler * IoHandlerPtr;

#include "io/IoState.h"
#include "io/IoObject.h"
#include "io/IoList.h"
#include "io/IoNumber.h"
#include "./game.h"
#include "../groups.h"
#include "../events/Numeric.h"
#include "../events/PlaneGetter.h"
#include "../events/String.h"

struct IoHandler {
    char * code;
	IoState * iostate;
	GamePtr game;

	size_t no_to_instantiate;
	IoObject ** to_instantiate;
	void ** addresses;
};

void IoHandler_init(IoHandlerPtr me, GamePtr game, const char * filename);
void IoHandler_load_file(IoHandlerPtr me, const char * filename);
void IoHandler_get_rooms(IoHandlerPtr me, GamePtr game);
void IoHandler_get_planes(IoHandlerPtr me, GamePtr game);
void IoHandler_connect_tiles(IoHandlerPtr me, GamePtr game);
void IoHandler_get_entities(IoHandlerPtr me, GamePtr game);
void IoHandler_get_mapped_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
);
void IoHandler_get_move_component(
	IoHandlerPtr me, IoObject * io_entity, EntityPtr entity
);
void IoHandler_get_sprite_component(
	IoHandlerPtr me, IoObject * io_entity, EntityPtr entity
);
void IoHandler_get_journey_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
);
void IoHandler_get_animate_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
);
void IoHandler_get_click_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
);
void IoHandler_get_interact_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
);
void IoHandler_get_speak_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
);
void IoHandler_process(IoHandlerPtr me, IoObject * object, void * address);
void IoHandler_get_scenarios(IoHandlerPtr me, GamePtr game);
void IoHandler_get_listener(
	IoHandlerPtr me, GamePtr game, IoObject * io_trigger, ListenerPtr trigger
);
void IoHandler_get_condition(
	IoHandlerPtr me, GamePtr game, IoObject * io_condition,
	ConditionPtr condition
);
void IoHandler_get_action(
	IoHandlerPtr me, GamePtr game, IoObject * io_action, ActionPtr action
);
void IoHandler_get_numeric(
	IoHandlerPtr me, GamePtr game, IoObject * io_numeric, NumericPtr numeric
);
void IoHandler_get_entity_getter(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity_getter,
	EntityGetterPtr entity_getter
);
void IoHandler_get_string(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity_getter,
	StringPtr string
);
void IoHandler_get_plane_getter(
	IoHandlerPtr me, GamePtr game, IoObject * io_plane_getter,
	PlaneGetterPtr plane_getter
);

RoomPtr IoHandler_get_room(IoHandlerPtr me, GamePtr game, IoObject * io_room);
PlanePtr IoHandler_get_plane(
	IoHandlerPtr me, GamePtr game, IoObject * io_plane
);
TilePtr IoHandler_get_tile(IoHandlerPtr me, GamePtr game, IoObject * io_tile);
void IoHandler_destroy(IoHandlerPtr me);

#endif
