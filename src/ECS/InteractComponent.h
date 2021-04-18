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

#ifndef __INTERACT_COMPONENT_H__
#define __INTERACT_COMPONENT_H__

#include "ECS.h"
#include "ClickComponent.h"
#include "../objects/font.h"

typedef struct {
	Component base_component;
    ClickComponentPtr click;
    TextBlockPtr hover_text;
} InteractComponent;

typedef InteractComponent * InteractComponentPtr;

ComponentPtr InteractComponent_init(
	void * me_void, EntityPtr entity, va_list * args
);
void InteractComponent_update(void * me_void);
void InteractComponent_draw(void * me_void);
void InteractComponent_delete(void * me_void);

#endif
