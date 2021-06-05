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

#include "./IoHandler.h"
#include "../ECS/Components.h"

void IoHandler_init(IoHandlerPtr me, GamePtr game, const char * filename) {
	me->game = game;

	me->iostate = IoState_new();
	IoState_init(me->iostate);

	me->no_to_instantiate = 0;
	me->to_instantiate = (IoObject **)malloc(0);
	me->addresses = (void **)malloc(0);

	IoHandler_load_file(me, filename);
}

void IoHandler_load_file(IoHandlerPtr me, const char * filename) {
	char * buffer = 0;
	long length;
	FILE * f = fopen(filename, "r");

	fseek(f, 0, SEEK_END);
	length = ftell(f) + 1;
	fseek(f, 0, SEEK_SET);
	buffer = malloc(length);
	if (buffer) {
		fread(buffer, 1, length, f);
	}
	fclose(f);
	buffer[length - 1] = '\0';

	IoState_doCString_(me->iostate, buffer);
}

void IoHandler_get_rooms(IoHandlerPtr me, GamePtr game) {
	IoObject *v = IoState_doCString_(me->iostate, "Get rooms");

	game->no_rooms = IoList_rawSize(v);
	me->game->rooms = (RoomPtr *) realloc(
		me->game->rooms, game->no_rooms * sizeof(RoomPtr)
	);
	IoSymbol * get_name = IoState_symbolWithCString_(me->iostate, "name");
	IoSymbol * get_focal_point = IoState_symbolWithCString_(
		me->iostate, "focal_point"
	);
	IoSymbol * get_scale_rate = IoState_symbolWithCString_(
		me->iostate, "scale_rate"
	);
	IoSymbol * get_tile_x = IoState_symbolWithCString_(
		me->iostate, "tile_x"
	);
	IoSymbol * get_is_current = IoState_symbolWithCString_(
		me->iostate, "is_current"
	);

	bool found_current = false;

	for (int i = 0; i < game->no_rooms; i ++) {
		IoObject * io_room = IoList_rawAt_(v, i);

		char * name = IoSeq_asCString(IoObject_getSlot_(io_room, get_name));
		int focal_point = IoNumber_asInt(IoObject_getSlot_(
			io_room, get_focal_point
		));
		double scale_rate = IoNumber_asDouble(IoObject_getSlot_(
			io_room, get_scale_rate
		));
		int tile_x = IoNumber_asInt(IoObject_getSlot_(
			io_room, get_tile_x
		));
		int is_current = IoNumber_asInt(IoObject_getSlot_(
			io_room, get_is_current
		));

		if (found_current && is_current) {
			printf("Found multiple rooms selected.\n");
			exit(-1);
		}
		found_current = found_current || is_current;

		me->game->rooms[i] = (RoomPtr) malloc(sizeof(Room));
		Room_init(me->game->rooms[i], name, focal_point, scale_rate, tile_x);
		if (is_current) {
			me->game->room = me->game->rooms[i];
		}
	}
}

void IoHandler_get_planes(IoHandlerPtr me, GamePtr game) {
	IoObject *v = IoState_doCString_(me->iostate, "Get planes");
	size_t no_planes = IoList_rawSize(v);
	IoSymbol * get_room = IoState_symbolWithCString_(me->iostate, "room");
	IoSymbol * get_name = IoState_symbolWithCString_(me->iostate, "name");
	IoSymbol * get_min_x = IoState_symbolWithCString_(me->iostate, "min_x");
	IoSymbol * get_min_y = IoState_symbolWithCString_(me->iostate, "min_y");
	IoSymbol * get_max_x = IoState_symbolWithCString_(me->iostate, "max_x");
	IoSymbol * get_max_y = IoState_symbolWithCString_(me->iostate, "max_y");
	IoSymbol * get_h = IoState_symbolWithCString_(me->iostate, "h");
	IoSymbol * get_h_x = IoState_symbolWithCString_(me->iostate, "hx");
	IoSymbol * get_h_y = IoState_symbolWithCString_(me->iostate, "hy");

	IoSymbol * get_x = IoState_symbolWithCString_(me->iostate, "x");
	IoSymbol * get_y = IoState_symbolWithCString_(me->iostate, "y");

	IoSymbol * get_unwalkable = IoState_symbolWithCString_(
		me->iostate, "unwalkable"
	);

	for (int i = 0; i < no_planes; i ++) {
		IoObject * io_plane = IoList_rawAt_(v, i);
		char * name = IoSeq_asCString(IoObject_getSlot_(io_plane, get_name));
		int min_x = IoNumber_asInt(IoObject_getSlot_(io_plane, get_min_x));
		int min_y = IoNumber_asInt(IoObject_getSlot_(io_plane, get_min_y));
		int max_x = IoNumber_asInt(IoObject_getSlot_(io_plane, get_max_x));
		int max_y = IoNumber_asInt(IoObject_getSlot_(io_plane, get_max_y));
		int h = IoNumber_asInt(IoObject_getSlot_(io_plane, get_h));
		int h_x = IoNumber_asInt(IoObject_getSlot_(io_plane, get_h_x));
		int h_y = IoNumber_asInt(IoObject_getSlot_(io_plane, get_h_y));

		const char * room_name = IoSeq_asCString(IoObject_getSlot_(
			IoObject_getSlot_(io_plane, get_room), get_name
		));
		RoomPtr room = Game_find_room(game, room_name);

		PlanePtr plane = Room_add_plane(
			room, name, min_x, min_y, max_x, max_y, h, h_x, h_y
		);

		IoObject * unwalkable = IoObject_getSlot_(io_plane, get_unwalkable);
		size_t no_unwalkable = IoList_rawSize(unwalkable);

		for (int j = 0; j < no_unwalkable; j ++) {
			IoObject * io_plane = IoList_rawAt_(unwalkable, j);
			int x = IoNumber_asInt(IoObject_getSlot_(io_plane, get_x));
			int y = IoNumber_asInt(IoObject_getSlot_(io_plane, get_y));
		    Tile_set_walkable(Plane_get_tile(plane, x, y), false);
		}
	}

	IoHandler_connect_tiles(me, game);
}

void IoHandler_connect_tiles(IoHandlerPtr me, GamePtr game) {
	IoObject *v = IoState_doCString_(me->iostate, "Get rooms");
	IoSymbol * get_tile_connections = IoState_symbolWithCString_(
		me->iostate, "tile_connections"
	);

	IoSymbol * get_name = IoState_symbolWithCString_(me->iostate, "name");
	IoSymbol * get_plane = IoState_symbolWithCString_(me->iostate, "plane");
	IoSymbol * get_x = IoState_symbolWithCString_(me->iostate, "x");
	IoSymbol * get_y = IoState_symbolWithCString_(me->iostate, "y");

	for (size_t i = 0; i < game->no_rooms; i ++) {
		IoObject * io_room = IoList_rawAt_(v, i);

		const char * room_name = IoSeq_asCString(IoObject_getSlot_(
			io_room, get_name
		));
		RoomPtr room = Game_find_room(game, room_name);

		IoObject * tile_connections = IoObject_getSlot_(
			io_room, get_tile_connections
		);

		size_t no_tile_connections = IoList_rawSize(tile_connections);

		for (size_t j = 0; j < no_tile_connections; j ++) {
			IoObject * tile_connection = IoList_rawAt_(tile_connections, j);
			IoObject * tile_1 = IoList_rawAt_(tile_connection, 0);
			IoObject * tile_2 = IoList_rawAt_(tile_connection, 1);

			char * plane_name_1 = IoSeq_asCString(IoObject_getSlot_(
				IoObject_getSlot_(tile_1, get_plane), get_name
			));
			PlanePtr plane_1 = Room_find_plane(room, plane_name_1);
			int x_1 = IoNumber_asInt(IoObject_getSlot_(tile_1, get_x));
			int y_1 = IoNumber_asInt(IoObject_getSlot_(tile_1, get_y));
			char * plane_name_2 = IoSeq_asCString(IoObject_getSlot_(
				IoObject_getSlot_(tile_2, get_plane), get_name
			));
			PlanePtr plane_2 = Room_find_plane(room, plane_name_2);
			int x_2 = IoNumber_asInt(IoObject_getSlot_(tile_2, get_x));
			int y_2 = IoNumber_asInt(IoObject_getSlot_(tile_2, get_y));

			Room_join_tiles(
				room, Plane_get_tile(plane_1, x_1, y_1),
				Plane_get_tile(plane_2, x_2, y_2)
			);
		}
	}
}

void IoHandler_get_entities(IoHandlerPtr me, GamePtr game) {
	IoObject *v = IoState_doCString_(me->iostate, "Get entities");
	size_t no_entities = IoList_rawSize(v);

	IoSymbol * get_name = IoState_symbolWithCString_(me->iostate, "name");
	IoSymbol * get_groups = IoState_symbolWithCString_(me->iostate, "groups");

	for (size_t i = 0; i < no_entities; i ++) {
		IoObject * io_entity = IoList_rawAt_(v, i);
		char * name = IoSeq_asCString(IoObject_getSlot_(io_entity, get_name));

	    EntityPtr entity = System_add_entity(game->sys, name);

		IoObject * groups = IoObject_getSlot_(io_entity, get_groups);
		size_t no_groups = IoList_rawSize(groups);

		for (size_t j = 0; j < no_groups; j ++) {
			char * group_name = IoSeq_asCString(IoList_rawAt_(groups, j));
			group_type group;
			if (strcmp(group_name, "player") == 0) {
				group = GROUP_PLAYERS;
			}
			else if (strcmp(group_name, "world") == 0) {
				group = GROUP_WORLD;
			}
			else {
				printf("Unknown group: '%s'\n", group_name);
				exit(-1);
			}
			Entity_add_group(entity, group);
		}
		IoHandler_get_mapped_component(me, game, io_entity, entity);
		IoHandler_get_move_component(me, io_entity, entity);
		IoHandler_get_sprite_component(me, io_entity, entity);
		IoHandler_get_journey_component(me, game, io_entity, entity);
		IoHandler_get_animate_component(me, game, io_entity, entity);
		IoHandler_get_click_component(me, game, io_entity, entity);
		IoHandler_get_interact_component(me, game, io_entity, entity);
		IoHandler_get_speak_component(me, game, io_entity, entity);
	}
}

void IoHandler_get_mapped_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
) {
	IoSymbol * get_mapped = IoState_symbolWithCString_(me->iostate, "mapped_c");
	IoObject * mapped = IoObject_getSlot_(io_entity, get_mapped);
	if (strcmp(IoObject_name(mapped), "nil") == 0) {
		return;
	}

	IoSymbol * get_x = IoState_symbolWithCString_(me->iostate, "x");
	IoSymbol * get_y = IoState_symbolWithCString_(me->iostate, "y");
	IoSymbol * get_plane = IoState_symbolWithCString_(me->iostate, "plane");

	double x = IoNumber_asDouble(IoObject_getSlot_(mapped, get_x));
	double y = IoNumber_asDouble(IoObject_getSlot_(mapped, get_y));
	IoObject * io_plane = IoObject_getSlot_(mapped, get_plane);

    Entity_add_component(
		entity, MAPPED_COMPONENT, 3, x, y,
		IoHandler_get_plane(me, game, io_plane)
	);
}

void IoHandler_get_move_component(
	IoHandlerPtr me, IoObject * io_entity, EntityPtr entity
) {
	IoSymbol * get_move = IoState_symbolWithCString_(me->iostate, "move_c");
	IoObject * move = IoObject_getSlot_(io_entity, get_move);
	if (strcmp(IoObject_name(move), "nil") == 0) {
		return;
	}

    Entity_add_component(entity, MOVE_COMPONENT, 0);
}

void IoHandler_get_sprite_component(
	IoHandlerPtr me, IoObject * io_entity, EntityPtr entity
) {
	IoSymbol * get_sprite = IoState_symbolWithCString_(
		me->iostate, "sprite_c"
	);
	IoObject * io_sprite = IoObject_getSlot_(io_entity, get_sprite);
	if (strcmp(IoObject_name(io_sprite), "nil") == 0) {
		return;
	}

	IoSymbol * get_img = IoState_symbolWithCString_(me->iostate, "img");
	IoSymbol * get_scaling = IoState_symbolWithCString_(
		me->iostate, "scaling"
	);

	char * img = IoSeq_asCString(IoObject_getSlot_(io_sprite, get_img));
	bool scaling = (bool)IoNumber_asInt(
		IoObject_getSlot_(io_sprite, get_scaling)
	);

	SpriteComponentPtr sprite = (SpriteComponentPtr)Entity_add_component(
		entity, SPRITE_COMPONENT, 1, img
	);
    SpriteComponent_enable_scaling(sprite, scaling);
}

void IoHandler_get_journey_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
) {
	IoSymbol * get_journey = IoState_symbolWithCString_(
		me->iostate, "journey_c"
	);
	IoObject * io_journey = IoObject_getSlot_(io_entity, get_journey);
	if (strcmp(IoObject_name(io_journey), "nil") == 0) {
		return;
	}

	IoSymbol * get_room = IoState_symbolWithCString_(
		me->iostate, "room"
	);
	IoObject * io_room = IoObject_getSlot_(io_journey, get_room);

	RoomPtr room = IoHandler_get_room(me, game, io_room);

	Entity_add_component(entity, JOURNEY_COMPONENT, 1, room);
}

void IoHandler_get_animate_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
) {
	IoSymbol * get_animate = IoState_symbolWithCString_(
		me->iostate, "animate_c"
	);
	IoObject * io_animate = IoObject_getSlot_(io_entity, get_animate);
	if (strcmp(IoObject_name(io_animate), "nil") == 0) {
		return;
	}

	IoSymbol * get_path = IoState_symbolWithCString_(me->iostate, "path");
	IoSymbol * get_scaling = IoState_symbolWithCString_(
		me->iostate, "scaling"
	);

	char * path = IoSeq_asCString(IoObject_getSlot_(io_animate, get_path));
	bool scaling = (bool)IoNumber_asInt(
		IoObject_getSlot_(io_animate, get_scaling)
	);

	AnimateComponentPtr animate = (AnimateComponentPtr)Entity_add_component(
		entity, ANIMATE_COMPONENT, 1, path
	);
    AnimateComponent_enable_scaling(animate, scaling);

	IoSymbol * get_images = IoState_symbolWithCString_(me->iostate, "images");

	IoObject * images = IoObject_getSlot_(io_animate, get_images);
	size_t no_images = IoList_rawSize(images);
	for (size_t i = 0; i < no_images; i ++) {
		IoObject * key_and_img = IoList_rawAt_(images, i);

		char * str_key = IoSeq_asCString(IoList_rawAt_(key_and_img, 0));
		char * img = IoSeq_asCString(IoList_rawAt_(key_and_img, 1));

	    AnimateComponent_load_sprite(animate, get_animation_key(str_key), img);
	}

	IoSymbol * get_current = IoState_symbolWithCString_(
		me->iostate, "current"
	);
	char * current = IoSeq_asCString(
		IoObject_getSlot_(io_animate, get_current)
	);
	AnimateComponent_use_sprite(animate, get_animation_key(current));
}

void IoHandler_get_click_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
) {
	IoSymbol * get_click = IoState_symbolWithCString_(
		me->iostate, "click_c"
	);
	IoObject * io_click = IoObject_getSlot_(io_entity, get_click);
	if (strcmp(IoObject_name(io_click), "nil") == 0) {
		return;
	}

	ClickComponentPtr click = (ClickComponentPtr)Entity_add_component(
		entity, CLICK_COMPONENT, 0
	);

	IoSymbol * get_tile = IoState_symbolWithCString_(me->iostate, "tile");
	IoSymbol * get_x = IoState_symbolWithCString_(me->iostate, "x");
	IoSymbol * get_y = IoState_symbolWithCString_(me->iostate, "y");
	IoSymbol * get_dir = IoState_symbolWithCString_(
		me->iostate, "direction"
	);

	TilePtr tile = IoHandler_get_tile(
		me, game, IoObject_getSlot_(io_click, get_tile)
	);
	double x = IoNumber_asDouble(IoObject_getSlot_(io_click, get_x));
	double y = IoNumber_asDouble(IoObject_getSlot_(io_click, get_y));
	directions dir = get_direction(
		IoSeq_asCString(IoObject_getSlot_(io_click, get_dir))
	);

	ClickComponent_set_interact_point(click, tile, x, y, dir);
}

void IoHandler_get_interact_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
) {
	IoSymbol * get_interact = IoState_symbolWithCString_(
		me->iostate, "interact_c"
	);
	IoObject * io_interact = IoObject_getSlot_(io_entity, get_interact);
	if (strcmp(IoObject_name(io_interact), "nil") == 0) {
		return;
	}

	IoSymbol * get_label = IoState_symbolWithCString_(me->iostate, "label");
	char * label = IoSeq_asCString(IoObject_getSlot_(io_interact, get_label));

	Entity_add_component(entity, INTERACT_COMPONENT, 1, label);
}

void IoHandler_get_speak_component(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity, EntityPtr entity
) {
	IoSymbol * get_speak = IoState_symbolWithCString_(me->iostate, "speak_c");
	IoObject * io_speak = IoObject_getSlot_(io_entity, get_speak);
	if (strcmp(IoObject_name(io_speak), "nil") == 0) {
		return;
	}

	Entity_add_component(entity, SPEAK_COMPONENT, 1, game->font);
}

void IoHandler_process(IoHandlerPtr me, IoObject * object, void * address) {
	me->no_to_instantiate ++;
	me->to_instantiate = (IoObject **)realloc(
		me->to_instantiate, me->no_to_instantiate * sizeof(IoObject *)
	);
	me->to_instantiate[me->no_to_instantiate - 1] = object;
	me->addresses = (void **)realloc(
		me->addresses, me->no_to_instantiate * sizeof(void *)
	);
	me->addresses[me->no_to_instantiate - 1] = address;
}

void IoHandler_get_scenarios(IoHandlerPtr me, GamePtr game) {
	IoObject *v = IoState_doCString_(me->iostate, "Get scenarios");
	size_t no_scenarios = IoList_rawSize(v);
	ScenarioManagerPtr sm = game->scenario_manager;

	IoSymbol * get_triggers = IoState_symbolWithCString_(
		me->iostate, "triggers"
	);
	IoSymbol * get_conditions = IoState_symbolWithCString_(
		me->iostate, "conditions"
	);
	IoSymbol * get_actions = IoState_symbolWithCString_(
		me->iostate, "actions"
	);

	for (size_t i = 0; i < no_scenarios; i ++) {
		IoObject * io_scenario = IoList_rawAt_(v, i);

		IoObject * io_triggers = IoObject_getSlot_(io_scenario, get_triggers);
		size_t no_triggers = IoList_rawSize(io_triggers);

		IoObject * io_conditions = IoObject_getSlot_(
			io_scenario, get_conditions
		);
		size_t no_conditions = IoList_rawSize(io_conditions);

		IoObject * io_actions = IoObject_getSlot_(io_scenario, get_actions);
		size_t no_actions = IoList_rawSize(io_actions);

		ScenarioPtr scenario = ScenarioManager_add_scenario(
			sm, no_triggers, no_conditions, no_actions
		);

		for (size_t j = 0; j < no_triggers; j ++) {
			IoObject * io_trigger = IoList_rawAt_(io_triggers, j);
			IoHandler_process(me, io_trigger, scenario->listeners[j]);
		}

		for (size_t j = 0; j < no_conditions; j ++) {
			IoObject * io_condition = IoList_rawAt_(io_conditions, j);
			IoHandler_process(me, io_condition, scenario->conditions[j]);
		}

		for (size_t j = 0; j < no_actions; j ++) {
			IoObject * io_action = IoList_rawAt_(io_actions, j);
			IoHandler_process(me, io_action, scenario->actions[j]);
		}
	}

	// Continues to instantiate objects until all scenarios are completed.
	while (me->no_to_instantiate > 0) {
		IoObject * object = me->to_instantiate[0];
		void * address = me->addresses[0];

		for (size_t i = 1; i < me->no_to_instantiate; i ++) {
			me->to_instantiate[i - 1] = me->to_instantiate[i];
			me->addresses[i - 1] = me->addresses[i];
		}
		me->no_to_instantiate --;
		me->to_instantiate = (IoObject **)realloc(
			me->to_instantiate, me->no_to_instantiate * sizeof(IoObject *)
		);
		me->addresses = (void **)realloc(
			me->addresses, me->no_to_instantiate * sizeof(void *)
		);

		const char * name = IoObject_name(object);

		if (strcmp(name, "TriggerObj") == 0) {
			IoHandler_get_listener(me, game, object, (ListenerPtr)address);
		}
		else if (strcmp(name, "ConditionObj") == 0) {
			IoHandler_get_condition(me, game, object, (ConditionPtr)address);
		}
		else if (strcmp(name, "ActionObj") == 0) {
			IoHandler_get_action(me, game, object, (ActionPtr)address);
		}
		else if (ISNUMBER(object) || strcmp(name, "NumericObj") == 0) {
			IoHandler_get_numeric(me, game, object, (NumericPtr)address);
		}
		else if (
			strcmp(name, "EntityObj") == 0
			|| strcmp(name, "EntityGetterObj") == 0
		) {
			IoHandler_get_entity_getter(
				me, game, object, (EntityGetterPtr)address
			);
		}
		else if (ISSEQ(object) || strcmp(name, "StringObj") == 0) {
			IoHandler_get_string(me, game, object, (StringPtr)address);
		}
		else if (strcmp(name, "PlaneObj") == 0) {
			IoHandler_get_plane_getter(
				me, game, object, (PlaneGetterPtr)address
			);
		}
		else {
			printf("IoObject found with unrecognized name: %s\n", name);
			IoObject_print(object);
			printf("\n");
			exit(-1);
		}
	}

	free(me->to_instantiate);
	free(me->addresses);
}

void IoHandler_get_listener(
	IoHandlerPtr me, GamePtr game, IoObject * io_trigger, ListenerPtr listener
) {
	IoSymbol * get_priority = IoState_symbolWithCString_(
		me->iostate, "priority"
	);
	IoSymbol * get_particulars = IoState_symbolWithCString_(
		me->iostate, "particulars"
	);

	IoObject * io_priority = IoObject_getSlot_(io_trigger, get_priority);
	int priority = IoNumber_asInt(io_priority);
	IoObject * io_particulars = IoObject_getSlot_(io_trigger, get_particulars);
	Listener_init(listener, io_particulars, me, game, priority);
}

void IoHandler_get_condition(
	IoHandlerPtr me, GamePtr game, IoObject * io_condition,
	ConditionPtr condition
) {
	IoSymbol * get_particulars = IoState_symbolWithCString_(
		me->iostate, "particulars"
	);
	IoObject * io_particulars = IoObject_getSlot_(
		io_condition, get_particulars
	);

	Condition_init(condition, io_particulars, me, game);
}

void IoHandler_get_action(
	IoHandlerPtr me, GamePtr game, IoObject * io_action, ActionPtr action
) {
	IoSymbol * get_particulars = IoState_symbolWithCString_(
		me->iostate, "particulars"
	);
	IoObject * io_particulars = IoObject_getSlot_(io_action, get_particulars);

	Action_init(action, io_particulars, me, game);
}

void IoHandler_get_numeric(
	IoHandlerPtr me, GamePtr game, IoObject * io_numeric, NumericPtr numeric
) {
	if (ISNUMBER(io_numeric)) {
		Numeric_init(numeric, io_numeric, me, game);
		return;
	}

	IoSymbol * get_particulars = IoState_symbolWithCString_(
		me->iostate, "particulars"
	);
	IoObject * io_particulars = IoObject_getSlot_(io_numeric, get_particulars);

	Numeric_init(numeric, io_particulars, me, game);
}

void IoHandler_get_entity_getter(
	IoHandlerPtr me, GamePtr game, IoObject * io_entity_getter,
	EntityGetterPtr entity_getter
) {
	if (strcmp(IoObject_name(io_entity_getter), "EntityObj") == 0) {
		EntityGetter_init(entity_getter, io_entity_getter, me, game);
		return;
	}

	IoSymbol * get_particulars = IoState_symbolWithCString_(
		me->iostate, "particulars"
	);
	IoObject * io_particulars = IoObject_getSlot_(
		io_entity_getter, get_particulars
	);

	EntityGetter_init(entity_getter, io_particulars, me, game);
}

void IoHandler_get_string(
	IoHandlerPtr me, GamePtr game, IoObject * io_string,
	StringPtr string
) {
	if (ISSEQ(io_string)) {
		String_init(string, io_string, me, game);
		return;
	}

	IoSymbol * get_particulars = IoState_symbolWithCString_(
		me->iostate, "particulars"
	);
	IoObject * io_particulars = IoObject_getSlot_(io_string, get_particulars);

	String_init(string, io_particulars, me, game);
}

void IoHandler_get_plane_getter(
	IoHandlerPtr me, GamePtr game, IoObject * io_plane_getter,
	PlaneGetterPtr plane_getter
) {
	if (strcmp(IoObject_name(io_plane_getter), "PlaneObj") == 0) {
		PlaneGetter_init(plane_getter, io_plane_getter, me, game);
		return;
	}

	IoSymbol * get_particulars = IoState_symbolWithCString_(
		me->iostate, "particulars"
	);
	IoObject * io_particulars = IoObject_getSlot_(
		io_plane_getter, get_particulars
	);

	PlaneGetter_init(plane_getter, io_particulars, me, game);
}

RoomPtr IoHandler_get_room(IoHandlerPtr me, GamePtr game, IoObject * io_room) {
	IoSymbol * get_name = IoState_symbolWithCString_(me->iostate, "name");
	char * room_name = IoSeq_asCString(IoObject_getSlot_(io_room, get_name));
	return Game_find_room(game, room_name);
}

PlanePtr IoHandler_get_plane(
	IoHandlerPtr me, GamePtr game, IoObject * io_plane
) {
	IoSymbol * get_name = IoState_symbolWithCString_(me->iostate, "name");
	IoSymbol * get_room = IoState_symbolWithCString_(me->iostate, "room");

	char * plane_name = IoSeq_asCString(IoObject_getSlot_(io_plane, get_name));
	IoObject * io_room = IoObject_getSlot_(io_plane, get_room);
	RoomPtr room = IoHandler_get_room(me, game, io_room);

	return Room_find_plane(room, plane_name);
}

TilePtr IoHandler_get_tile(IoHandlerPtr me, GamePtr game, IoObject * io_tile) {
	IoSymbol * get_plane = IoState_symbolWithCString_(me->iostate, "plane");
	IoSymbol * get_x = IoState_symbolWithCString_(me->iostate, "x");
	IoSymbol * get_y = IoState_symbolWithCString_(me->iostate, "y");

	IoObject * io_plane = IoObject_getSlot_(io_tile, get_plane);
	IoObject * io_x = IoObject_getSlot_(io_tile, get_x);
	IoObject * io_y = IoObject_getSlot_(io_tile, get_y);

	PlanePtr plane = IoHandler_get_plane(me, game, io_plane);
	int x = IoNumber_asInt(io_x);
	int y = IoNumber_asInt(io_y);

	return Plane_get_tile(plane, x, y);
}

void IoHandler_destroy(IoHandlerPtr me) {
	IoState_free(me->iostate);
}
