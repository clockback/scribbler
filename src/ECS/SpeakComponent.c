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

#include "SpeakComponent.h"

ComponentPtr SpeakComponent_init(
	void * me_void, EntityPtr entity, va_list * args
) {
	SpeakComponentPtr me = (SpeakComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, SPEAK_COMPONENT);

	me->font = va_arg(*args, FontPtr);
	me->duration = 0;
	me->text_block = NULL;
	me->mapped = (MappedComponentPtr)Entity_fetch_component(
		entity, MAPPED_COMPONENT
	);
	me->animate = (AnimateComponentPtr)Entity_fetch_component(
		entity, ANIMATE_COMPONENT
	);

	return me_component;
}

void SpeakComponent_update(void * me_void) {
	SpeakComponentPtr me = (SpeakComponentPtr)me_void;

	if (me->duration > 0) {
		me->duration --;

		int x = me->animate->dest_rect->x + me->animate->dest_rect->w / 2;
		int y = me->animate->dest_rect->y - 5;
		TextBlock_position(me->text_block, x, y, ANCHOR_BOTTOM_CENTRE);
	}
	else if (me->text_block != NULL) {
		TextBlock_destroy(me->text_block);
		me->text_block = NULL;
	}
}

void SpeakComponent_draw(void * me_void) {
}

void SpeakComponent_delete(void * me_void) {
}

void SpeakComponent_speak(SpeakComponentPtr me, char * text, int duration) {
	me->text_block = Font_add_text(me->font, text);
	int x = me->animate->dest_rect->x + me->animate->dest_rect->w / 2;
	int y = me->animate->dest_rect->y - 5;
	TextBlock_position(me->text_block, x, y, ANCHOR_BOTTOM_CENTRE);
	me->duration = duration;
}
