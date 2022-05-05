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

#ifndef __ACTION_H__
#define __ACTION_H__

#include <stdarg.h>
#include <stddef.h>

typedef enum {
	ENTITY_JOURNEY_TO,
	SET_USER_INPUT,
	WAIT,
	SPEAK,
	MAX_ACTIONS
} ActionType;

extern size_t action_sizes[MAX_ACTIONS];

void pre_init_action_functions();
void pre_init_action_sizes();
void pre_init_action_run();
void init_actions();

#include "actions/EntityJourneyToAction.h"
#include "actions/SetUserInputAction.h"
#include "actions/WaitAction.h"
#include "actions/SpeakAction.h"

extern void (*init_for_action_functions[MAX_ACTIONS]) (
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);

extern bool (*run_for_action_functions[MAX_ACTIONS]) (void * me_void);


#endif
