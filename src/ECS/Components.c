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

#include <stdio.h>

#include "Components.h"
#include "ECS.h"

ComponentPtr (*init_for_component_functions[MAX_COMPONENTS]) (
	void* me_void, EntityPtr entity, va_list * args
);

size_t component_sizes[MAX_COMPONENTS];

void (*update_for_component_functions[MAX_COMPONENTS]) (void* me_void);

void (*draw_for_component_functions[MAX_COMPONENTS]) (void* me_void);

void (*delete_for_component_functions[MAX_COMPONENTS]) (void* me_void);

void pre_init_functions() {
	init_for_component_functions[MAPPED_COMPONENT] = MappedComponent_init;
	init_for_component_functions[MOVE_COMPONENT] = MoveComponent_init;
	init_for_component_functions[SPRITE_COMPONENT] = SpriteComponent_init;
	init_for_component_functions[JOURNEY_COMPONENT] = JourneyComponent_init;
	init_for_component_functions[ANIMATE_COMPONENT] = AnimateComponent_init;
	init_for_component_functions[CLICK_COMPONENT] = ClickComponent_init;
}

void pre_init_sizes() {
	component_sizes[MAPPED_COMPONENT] = sizeof(MappedComponent);
	component_sizes[MOVE_COMPONENT] = sizeof(MoveComponent);
	component_sizes[SPRITE_COMPONENT] = sizeof(SpriteComponent);
	component_sizes[JOURNEY_COMPONENT] = sizeof(JourneyComponent);
	component_sizes[ANIMATE_COMPONENT] = sizeof(AnimateComponent);
	component_sizes[CLICK_COMPONENT] = sizeof(ClickComponent);
}

void pre_update_functions() {
	update_for_component_functions[MAPPED_COMPONENT] = MappedComponent_update;
	update_for_component_functions[MOVE_COMPONENT] = MoveComponent_update;
	update_for_component_functions[SPRITE_COMPONENT] = SpriteComponent_update;
	update_for_component_functions[JOURNEY_COMPONENT] = JourneyComponent_update;
	update_for_component_functions[ANIMATE_COMPONENT] = AnimateComponent_update;
	update_for_component_functions[CLICK_COMPONENT] = ClickComponent_update;
}

void pre_draw_functions() {
	draw_for_component_functions[MAPPED_COMPONENT] = MappedComponent_draw;
	draw_for_component_functions[MOVE_COMPONENT] = MoveComponent_draw;
	draw_for_component_functions[SPRITE_COMPONENT] = SpriteComponent_draw;
	draw_for_component_functions[JOURNEY_COMPONENT] = JourneyComponent_draw;
	draw_for_component_functions[ANIMATE_COMPONENT] = AnimateComponent_draw;
	draw_for_component_functions[CLICK_COMPONENT] = ClickComponent_draw;
}

void pre_delete_functions() {
	delete_for_component_functions[MAPPED_COMPONENT] = MappedComponent_delete;
	delete_for_component_functions[MOVE_COMPONENT] = MoveComponent_delete;
	delete_for_component_functions[SPRITE_COMPONENT] = SpriteComponent_delete;
	delete_for_component_functions[JOURNEY_COMPONENT] = JourneyComponent_delete;
	delete_for_component_functions[ANIMATE_COMPONENT] = AnimateComponent_delete;
	delete_for_component_functions[CLICK_COMPONENT] = ClickComponent_delete;
}

void init_components() {
	pre_init_functions();
	pre_update_functions();
	pre_draw_functions();
	pre_delete_functions();
	pre_init_sizes();
}
