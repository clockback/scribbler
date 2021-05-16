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

#ifndef __TRIGGER_H__
#define __TRIGGER_H__

#include <stdarg.h>
#include <stddef.h>

typedef enum {
	INTERACT_ENTITY,
	MAX_TRIGGERS
} TriggerType;

extern size_t trigger_sizes[MAX_TRIGGERS];

void pre_init_trigger_functions();
void pre_init_trigger_sizes();
void init_triggers();

#include "triggers/InteractEntityTrigger.h"

extern void (*init_for_trigger_functions[MAX_TRIGGERS]) (
	void * me_void, va_list * args
);


#endif
