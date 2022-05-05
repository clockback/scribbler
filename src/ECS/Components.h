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

#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include "MappedComponent.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "JourneyComponent.h"
#include "AnimateComponent.h"
#include "ClickComponent.h"
#include "InteractComponent.h"
#include "SpeakComponent.h"
#include "ECS.h"

extern ComponentPtr (*init_for_component_functions[MAX_COMPONENTS]) (
	void* me_void, EntityPtr entity, va_list * args
);
extern size_t component_sizes[MAX_COMPONENTS];
extern void (*update_for_component_functions[MAX_COMPONENTS]) (void* me_void);
extern void (*draw_for_component_functions[MAX_COMPONENTS]) (void* me_void);
extern void (*delete_for_component_functions[MAX_COMPONENTS]) (void* me_void);


void pre_init_functions();
void pre_init_sizes();
void pre_update_functions();
void pre_draw_functions();
void init_components();

#endif
