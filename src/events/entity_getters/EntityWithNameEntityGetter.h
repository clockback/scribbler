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

#ifndef __ENTITYWITHNAMEENTITYGETTER_H__
#define __ENTITYWITHNAMEENTITYGETTER_H__

#include "../EntityGetter.h"
#include "../../ECS/ECS.h"
#include "../../objects/game.h"
#include "../String.h"

typedef struct {
    GamePtr game;
    StringPtr name;
} EntityWithNameEntityGetter;

typedef EntityWithNameEntityGetter * EntityWithNameEntityGetterPtr;

void EntityWithNameEntityGetter_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);
EntityPtr EntityWithNameEntityGetter_evaluate(void * me);

#endif
