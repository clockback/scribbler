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

#ifndef __SPEAK_COMPONENT_H__
#define __SPEAK_COMPONENT_H__

typedef struct SpeakComponent SpeakComponent;
typedef SpeakComponent * SpeakComponentPtr;

#include "ECS.h"
#include "MappedComponent.h"
#include "AnimateComponent.h"
#include "../objects/font.h"

struct SpeakComponent {
	Component base_component;
	FontPtr font;
	int duration;
	TextBlockPtr text_block;
	MappedComponentPtr mapped;
	AnimateComponentPtr animate;
};

ComponentPtr SpeakComponent_init(
	void * me_void, EntityPtr entity, va_list * args
);
void SpeakComponent_update(void * me_void);
void SpeakComponent_draw(void * me_void);
void SpeakComponent_delete(void * me_void);

void SpeakComponent_speak(SpeakComponentPtr speak, char * text, int duration);

#endif
