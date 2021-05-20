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

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "ECS.h"
#include "Components.h"
#include "../objects/globals.h"

void Component_init(ComponentPtr me, EntityPtr entity, ComponentID type) {
	me->type = type;
	me->entity = entity;
}

void Component_update(ComponentPtr me) {
	update_for_component_functions[me->type](me);
}

void Component_draw(ComponentPtr me) {
	draw_for_component_functions[me->type](me);
}

void Component_delete(ComponentPtr me) {
	EntityPtr entity = me->entity;
	entity->has_component[me->type] = false;
	delete_for_component_functions[me->type](me);
	free(me);
}


void Entity_init(EntityPtr me, SystemPtr system, ScreenPtr screen, char * id) {
	me->active = true;
	me->system = system;
	for (ComponentID i = 0; i < MAX_COMPONENTS; i ++) {
		me->has_component[i] = false;
	}
	me->screen = screen;
	me->id = (char *) malloc(strlen(id) * sizeof(char));
	strcpy(me->id, id);
}

void * Entity_add_component(EntityPtr me, ComponentID type, int no_args, ...) {
	me->has_component[type] = true;
	me->components[type] = (void*) malloc(component_sizes[type]);

	va_list args;
	va_start(args, no_args);
	init_for_component_functions[type](
		Entity_fetch_component(me, type), me, &args
	);
	va_end(args);

	return Entity_fetch_component(me, type);
}

bool Entity_has_component(EntityPtr me, ComponentID type) {
	return me->has_component[type];
}

void * Entity_fetch_component(EntityPtr me, ComponentID type) {
	return me->components[type];
}

void Entity_add_group(EntityPtr me, Group group) {
	me->in_group[group] = true;
	System_add_to_group(me->system, me, group);
}

void Entity_delete_group(EntityPtr me, Group group) {
	me->in_group[group] = false;
	bool found = false;
	for (size_t i = 0; i < MAX_COMPONENTS; i ++) {
		if (me->system->grouped_entities[group][i] == me) {
			found = true;
		}
		else if (found) {
			me->system->grouped_entities[group][i - 1] = (
				me->system->grouped_entities[group][i]
			);
		}
	}
	me->system->grouped_entity_sizes[group] --;
	me->system->grouped_entities[group] = realloc(
		me->system->grouped_entities[group],
		me->system->grouped_entity_sizes[group] * sizeof(EntityPtr)
	);
}

void Entity_update(EntityPtr me) {
	for (size_t type = 0; type < MAX_COMPONENTS; type ++) {
		if (Entity_has_component(me, type)) {
			Component_update((ComponentPtr)Entity_fetch_component(me, type));
		}
	}
}

void Entity_draw(EntityPtr me) {
	for (size_t type = 0; type < MAX_COMPONENTS; type ++) {
		if (Entity_has_component(me, type)) {
			Component_draw((ComponentPtr)Entity_fetch_component(me, type));
		}
	}
}

void Entity_delete(EntityPtr me) {
	for (ComponentID type = 0; type < MAX_COMPONENTS; type ++) {
		Component_delete(me->components[type]);
		free(me->components[type]);
	}
	free(me);
}

void System_init(SystemPtr me, GlobalsPtr globals) {
	init_components();

    me->no_entities = 0;
    me->entities = (EntityPtr*) malloc(0);
	me->entities = (EntityPtr *) realloc(
		me->entities, (me->no_entities + 1) * sizeof(EntityPtr)
	);
    for (size_t i = 0; i < MAX_GROUPS; i ++) {
    	me->grouped_entities[i] = (EntityPtr*) malloc(sizeof(EntityPtr));
    	me->grouped_entity_sizes[i] = 0;
    }
    me->screen = globals->screen;
    me->hover_entity = NULL;
    me->globals = globals;

    System_accept_user_input(me, true);
}

EntityPtr System_add_entity(SystemPtr me, char * id) {
	me->entities = (EntityPtr *) realloc(
		me->entities, (me->no_entities + 1) * sizeof(EntityPtr)
	);
	EntityPtr entity = (EntityPtr) malloc(sizeof(Entity));
	me->entities[me->no_entities] = entity;
	me->no_entities ++;
	Entity_init(entity, me, me->screen, id);

	return entity;
}

void System_add_to_group(SystemPtr me, EntityPtr entity, Group group) {
	me->grouped_entities[group] = realloc(
		me->grouped_entities[group],
		(me->grouped_entity_sizes[group] + 1) * sizeof(EntityPtr)
	);
	me->grouped_entities[group][me->grouped_entity_sizes[group]] = entity;
	me->grouped_entity_sizes[group] ++;
}

EntityPtr * System_get_group(SystemPtr me, Group group) {
	return me->grouped_entities[group];
}

size_t System_get_group_size(SystemPtr me, Group group) {
	return me->grouped_entity_sizes[group];
}

void System_update(SystemPtr me) {
	for (size_t i = 0; i < me->no_entities; i ++) {
		EntityPtr entity = me->entities[i];
		if (entity->active) {
			Entity_update(entity);
		}
	}
}

void System_draw(SystemPtr me) {
	for (size_t i = 0; i < me->no_entities; i ++) {
		EntityPtr entity = me->entities[i];
		if (entity->active) {
			Entity_draw(entity);
		}
	}
}

void System_refresh(SystemPtr me) {
	for (size_t i = 0; i < MAX_GROUPS; i ++) {
		EntityPtr * group = me->grouped_entities[i];
		size_t to_shift = 0;
		size_t orig_size = me->grouped_entity_sizes[i];
		for (size_t j = 0; j < orig_size; j ++) {
			if (!group[j]->active) {
				to_shift ++;
				me->grouped_entity_sizes[i] --;
			}
			else {
				group[j - to_shift] = group[j];
			}
		}
		me->grouped_entities[i] = realloc(
			me->grouped_entities[i],
			me->grouped_entity_sizes[i] * sizeof(EntityPtr)
		);
	}

	size_t to_shift = 0;
	size_t orig_size = me->no_entities;
	for (size_t i = 0; i < orig_size; i ++) {
		if (!me->entities[i]->active) {
			to_shift ++;
			Entity_delete(me->entities[i]);
			me->no_entities --;
		}
		else {
			me->entities[i - to_shift] = me->entities[i];
		}
	}
	me->entities = realloc(me->entities, me->no_entities * sizeof(EntityPtr));
}

void System_accept_user_input(SystemPtr me, bool accept) {
	me->accept_user_input = accept;
}

bool System_is_accepting_user_input(SystemPtr me) {
	return me->accept_user_input;
}
