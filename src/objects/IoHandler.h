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

#ifndef __IO_HANDLER_H__
#define __IO_HANDLER_H__

#include "io/IoState.h"
#include "io/IoObject.h"
#include "io/IoList.h"
#include "io/IoNumber.h"

#include "./game.h"
#include "../groups.h"

typedef struct {
    char * code;
	IoState *iostate;
	GamePtr game;
} IoHandler;

typedef IoHandler * IoHandlerPtr;

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

RoomPtr IoHandler_get_room(IoHandlerPtr me, GamePtr game, IoObject * io_room);
PlanePtr IoHandler_get_plane(
	IoHandlerPtr me, GamePtr game, IoObject * io_plane
);
TilePtr IoHandler_get_tile(IoHandlerPtr me, GamePtr game, IoObject * io_tile);
void IoHandler_destroy(IoHandlerPtr me);

#endif
