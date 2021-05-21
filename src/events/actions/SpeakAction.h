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

#ifndef __SPEAKACTION_H__
#define __SPEAKACTION_H__

typedef struct SpeakAction SpeakAction;
typedef SpeakAction * SpeakActionPtr;

#include "../Action.h"
#include "../String.h"
#include "../../ECS/SpeakComponent.h"

struct SpeakAction {
	EntityGetterPtr entity;
	StringPtr text;
	NumericPtr duration;
	int i;
	bool started;
};

void SpeakAction_init(void * me, va_list * args);
bool SpeakAction_run(void * me);

#endif
