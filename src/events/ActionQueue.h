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

#ifndef __ACTIONQUEUE_H__
#define __ACTIONQUEUE_H__

typedef struct ActionQueue ActionQueue;
typedef ActionQueue * ActionQueuePtr;

#include "stddef.h"
#include "ScenarioManager.h"

struct ActionQueue {
	size_t no_actions;
	ActionPtr * actions;
};

void ActionQueue_init(
	ActionQueuePtr me, ActionPtr * actions, size_t no_actions
);
bool ActionQueue_run(ActionQueuePtr me);


#endif
