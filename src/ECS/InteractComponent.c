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

#include <stdarg.h>

#include "InteractComponent.h"
#include "ECS.h"

ComponentPtr InteractComponent_init(
	void * me_void, EntityPtr entity, va_list * args
) {
	InteractComponentPtr me = (InteractComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, INTERACT_COMPONENT);

	me->click = (ClickComponentPtr)Entity_fetch_component(
		entity, CLICK_COMPONENT
	);
	me->hover_text = Font_add_text(
		entity->system->globals->font, va_arg(*args, char *)
	);
	TextBlock_position(me->hover_text, 0, 0, ANCHOR_TOP_LEFT);
	me->hover_text->visible = true;

	return me_component;
}

void InteractComponent_update(void * me_void) {
}

void InteractComponent_draw(void * me_void) {
	InteractComponentPtr me = (InteractComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;

	int x, y;
	SDL_GetMouseState(&x, &y);

	x /= me_component->entity->system->screen->scale;
	y /= me_component->entity->system->screen->scale;

	bool is_hovering = ClickComponent_targeting(me->click, x, y);
	me->hover_text->visible = is_hovering;
	if (is_hovering) {
		TextBlock_position(me->hover_text, x, y - 10, ANCHOR_BOTTOM_CENTRE);
		me_component->entity->system->hover_entity = me_component->entity;
	}
}

void InteractComponent_delete(void * me_void) {
	InteractComponentPtr me = (InteractComponentPtr)me_void;
	TextBlock_destroy(me->hover_text);
}
