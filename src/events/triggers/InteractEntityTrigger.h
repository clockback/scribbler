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

#ifndef __INTERACTENTITYTRIGGER_H__
#define __INTERACTENTITYTRIGGER_H__

#include <stdbool.h>

#include "io/IoState.h"

typedef struct InteractEntityTrigger InteractEntityTrigger;
typedef InteractEntityTrigger * InteractEntityTriggerPtr;

#include "../../ECS/ECS.h"
#include "../ScenarioManager.h"
#include "../../objects/IoHandler.h"

struct InteractEntityTrigger {
    EntityPtr object;
};

void InteractEntityTrigger_init(void * me_void, va_list * args);

void InteractEntityTrigger_listener_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);

#endif
