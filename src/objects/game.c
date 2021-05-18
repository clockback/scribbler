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

#include "./game.h"
#include "./graphics.h"
#include "./room.h"
#include "./globals.h"
#include "../ECS/Components.h"
#include "./IoHandler.h"

void Game_init(GamePtr me, int screen_width, int screen_height) {
	me->running = true;

	me->screen = (ScreenPtr) malloc(sizeof(Screen));
    Screen_init(me->screen, screen_width, screen_height, 2);

    me->rooms = (RoomPtr *) malloc(sizeof(RoomPtr));

    me->font = (FontPtr) malloc(sizeof(Font));
    Font_init(me->font, "assets/images/glyphs", me->screen, 100);

    me->scenario_manager = (ScenarioManagerPtr) malloc(sizeof(ScenarioManager));
    ScenarioManager_init(me->scenario_manager);

	me->globals = (GlobalsPtr) malloc(sizeof(Globals));
	Globals_init(
		me->globals, me->screen, me->room, me->font, me->scenario_manager
	);

    me->rend = me->screen->rend;

	me->sys = (SystemPtr) malloc(sizeof(System));
    System_init(me->sys, me->globals);

    me->event = (SDL_Event *) malloc(sizeof(SDL_Event));

    Game_prepare(me);

    me->cursor_src_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    me->cursor_dest_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

    SDL_ShowCursor(SDL_DISABLE);

	me->plain_cursor = Screen_load_sprite(
		me->screen, "assets/images/interface/cursors/plain.png",
		&me->cursor_src_rect->w, &me->cursor_src_rect->h
	);
	me->interact_cursor = Screen_load_sprite(
		me->screen, "assets/images/interface/cursors/interact.png",
		&me->cursor_dest_rect->w, &me->cursor_dest_rect->h
	);
	me->cursor_src_rect->x = 0;
	me->cursor_src_rect->y = 0;
	me->cursor_dest_rect->x = 0;
	me->cursor_dest_rect->y = 0;
}

void Game_prepare(GamePtr me) {
	IoHandlerPtr io = (IoHandlerPtr) malloc(sizeof(IoHandler));
	IoHandler_init(io, me, "src/io/utils.io");
	IoHandler_load_file(io, "src/io/game.io");
	IoHandler_get_rooms(io, me);
	IoHandler_get_planes(io, me);
	IoHandler_get_entities(io, me);

	IoHandler_destroy(io);

	ScenarioPtr scenario = ScenarioManager_add_scenario(me->scenario_manager);
	Scenario_add_listener(
		scenario, INTERACT_ENTITY, 0, 1, Game_find_entity(me, "Rubbish bin")
	);
	StringPtr s1 = Scenario_add_string(scenario, RAW_STRING, 1, "Micah");
	EntityGetterPtr e1 = Scenario_add_entity_getter(
		scenario, ENTITY_WITH_NAME, 2, me, s1
	);
	NumericPtr n1 = Scenario_add_numeric(scenario, ENTITY_GET_X, 1, e1);
	NumericPtr n2 = Scenario_add_numeric(scenario, RAW_NUMERIC, 1, 1.0);
	Scenario_add_condition(scenario, NUMERIC_EQUAL_TO, 2, n1, n2);
}

void Game_handle_events(GamePtr me) {
    while (SDL_PollEvent(me->event)) {
        switch (me->event->type) {
        case SDL_QUIT:
            me->running = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
        	Game_click(me);
        	break;
        default:
            break;
        }
    }
}

void Game_update(GamePtr me) {
	System_refresh(me->sys);
	System_update(me->sys);
	Game_handle_scenarios(me);

	me->sys->hover_entity = NULL;

	SDL_GetMouseState(&me->cursor_dest_rect->x, &me->cursor_dest_rect->y);
	me->cursor_dest_rect->x /= me->screen->scale;
	me->cursor_dest_rect->y /= me->screen->scale;
}

void Game_handle_scenarios(GamePtr me) {
	ScenarioManager_handle_scenarios(me->scenario_manager);
	ScenarioManager_clean_triggers(me->scenario_manager);
}

void Game_render(GamePtr me) {
    SDL_SetRenderTarget(me->screen->rend, me->screen->main_tex);
    SDL_SetRenderDrawColor(me->rend, 255, 0, 0, 255);
    SDL_RenderClear(me->screen->rend);

	for (size_t i = 0; i < me->room->no_planes; i ++) {
		Plane_draw(me->room->planes[i], me->screen);
	}

	EntityPtr * world_entities = System_get_group(me->sys, GROUP_WORLD);
	size_t no_world_entities = System_get_group_size(me->sys, GROUP_WORLD);
	EntityPtr * draw_order = (EntityPtr *) malloc(
		no_world_entities * sizeof(EntityPtr)
	);
	for (size_t i = 0; i < no_world_entities; i ++) {
		draw_order[i] = world_entities[i];
	}

	quick_sort(draw_order, no_world_entities);

	for (size_t i = 0; i < no_world_entities; i ++) {
		Entity_draw(draw_order[i]);
	}

	for (int i = 0; i < me->font->no_text_blocks; i ++) {
		TextBlockPtr text_block = me->font->text_blocks[i];
		if (text_block->visible) {
			SDL_RenderCopy(
				me->screen->rend, text_block->image, text_block->rect,
				text_block->dest_rect
			);
		}
	}

	SDL_Texture * cursor;
	if (me->sys->hover_entity == NULL) {
		cursor = me->plain_cursor;
	}
	else {
		cursor = me->interact_cursor;
		me->cursor_dest_rect->x -= me->cursor_dest_rect->w / 2;
		me->cursor_dest_rect->y -= me->cursor_dest_rect->h / 2;
	}
	SDL_RenderCopy(
		me->screen->rend, cursor, me->cursor_src_rect, me->cursor_dest_rect
	);

    SDL_SetRenderTarget(me->screen->rend, NULL);
	SDL_RenderCopy(
		me->screen->rend, me->screen->main_tex, me->screen->src_rect,
		me->screen->dest_rect
	);
	SDL_RenderPresent(me->screen->rend);
}

void Game_clean(GamePtr me) {
	SDL_DestroyWindow(me->screen->window);
	SDL_DestroyRenderer(me->rend);
	SDL_Quit();
}

void Game_click(GamePtr me) {
    int rc;

    /* Finds the position of the mouse. */
	int mouse_x = me->event->motion.x / me->screen->scale;
	int mouse_y = me->event->motion.y / me->screen->scale;

	/* Finds all the entities to be drawn and sorts them. */
	EntityPtr * world_entities = System_get_group(me->sys, GROUP_WORLD);
	size_t no_world_entities = System_get_group_size(me->sys, GROUP_WORLD);
	EntityPtr * draw_order = (EntityPtr *) malloc(
		no_world_entities * sizeof(EntityPtr)
	);
	for (size_t i = 0; i < no_world_entities; i ++) {
		draw_order[i] = world_entities[i];
	}
	quick_sort(draw_order, no_world_entities);

	/**
	 * Iterates over each entity, and if the mouse is hovering over the entity,
	 * allows the entity to handle the event.
     */
	for (size_t i = no_world_entities - 1; (int)i >= 0; i --) {
		if (Entity_has_component(draw_order[i], CLICK_COMPONENT)) {
			ClickComponentPtr click_component = Entity_fetch_component(
				draw_order[i], CLICK_COMPONENT
			);
			if (ClickComponent_targeting(click_component, mouse_x, mouse_y)) {
				ClickComponent_journey_to(click_component, mouse_x, me->room);
				return;
			}
		}
	}

	/* Finds the player entity/entities. */
	EntityPtr * players = System_get_group(me->sys, GROUP_PLAYERS);
	size_t no_players = System_get_group_size(me->sys, GROUP_PLAYERS);

	/**
	 * Begins searching for the most appropriate location to which the player
	 * entity should travel.
	 */
	int no_plane = -1;
	double chosen_x, chosen_y;
	double chosen_height = -999999;
	TilePtr chosen_tile = NULL;

	/**
	 * Iterates over each plane to see if it is a better match to the user's
	 * command than the best plane found so far.
	 */
	for (int i = 0; i < me->room->no_planes; i ++) {
		PlanePtr plane = me->room->planes[i];

		/* Finds the y-coordinate of the mouse on the plane. */
    	double y = Plane_get_mapped_y(
			plane, me->screen, mouse_x, mouse_y, &rc
		);

    	/* If no y-coordinate could be found, ignores the plane. */
    	if (rc != 0) {
    		continue;
    	}

    	/* Finds the x-coordinate of the mouse on the plane. */
		double x = Plane_get_mapped_x(plane, me->screen, mouse_x, y);
		if (x < plane->min_x || x > plane->max_x) {
			continue;
		}

		/* Finds the tile which the user clicked on. */
		TilePtr tile = Plane_get_tile(plane, (int)x, (int)y);

		/**
		 * If the tile is unwalkable, it checks the nearest neighbouring tile
		 * to the selected tile.
		 */
		if (!tile->walkable) {
			/**
			 * Finds whether the mouse is closer to selecting a horizontally
			 * adjacent tile or a vertically adjacent tile.
			 */
			double close_x = pow(x - (int)x - 0.5, 2);
			double close_y = pow(y - (int)y - 0.5, 2);
			double other_x;
			double other_y;

			/* Considers the horizontally adjacent tile if it is closer. */
			if (close_x >= close_y) {
				/**
				 * Uses the same y-coordinate as before. Only the x-coordinate
				 * is changed.
				 */
				other_y = y;

				/**
				 * If the x-coordinate is on the left-hand side of the tile,
				 * and the tile is not left of the border, chooses the tile.
				 */
				if (x - (int)x <= 0.5 && (int)x - 1 > plane->min_x) {
					other_x = (double)(int)x;
					tile = Plane_get_tile(plane, (int)x - 1, (int)y);
				}

				/**
				 * If the x-coordinate is on the right-hand side of the tile,
				 * and the tile is not right of the border, chooses the tile.
				 */
				else if (x - (int)x > 0.5 && (int)x + 1 < plane->max_x) {
					other_x = (double)(int)x + 1;
					tile = Plane_get_tile(plane, (int)x + 1, (int)y);
				}

				/**
				 * Discards the plane if the tile is not placed
				 * conveniently.
				 */
				else {
					continue;
				}
			}

			/* Considers the vertically adjacent tile if it is closer. */
			else {
				/**
				 * Uses the same x-coordinate as before. Only the y-coordinate
				 * is changed.
				 */
				other_x = x;

				/**
				 * If the y-coordinate is below the tile, and the tile is not
				 * below the border, chooses the tile.
				 */
				if (y - (int)y <= 0.5 && (int)y - 1 > plane->min_y) {
					other_y = (double)(int)y;
					tile = Plane_get_tile(plane, (int)x, (int)y - 1);
				}

				/**
				 * If the y-coordinate is above the tile, and the tile is not
				 * above the border, chooses the tile.
				 */
				else if (y - (int)y > 0.5 && (int)y + 1 < plane->max_y) {
					other_y = (double)(int)y + 1;
					tile = Plane_get_tile(plane, (int)x, (int)y + 1);
				}

				/**
				 * Discards the plane if the tile is not placed
				 * conveniently.
				 */
				else {
					continue;
				}
			}

			/**
			 * If the tile is walkable, walks to the modified position of the
			 * mouse.
			 */
			if (tile->walkable) {
				x = other_x;
				y = other_y;
			}

			/* It the tile is not walkable, discards the tile. */
			else {
				continue;
			}
		}

		/* Finds the position of the plane at the location chosen. */
		double height = Plane_get_height(plane, x, y);

		/**
		 * If the height of the plane is above the previously best-found plane
		 * which accommodates the mouse position, uses the plane and the click
		 * position.
		 */
		if (height > chosen_height) {
			no_plane = i;
			chosen_x = x;
			chosen_y = y;
			chosen_height = height;
			chosen_tile = tile;;
		}
	}

	/**
	 * If no plane has been found which accommodates the event, discards the
	 * mouse click.
	 */
	if (no_plane == -1) {
		return;
	}

	/**
	 * Commands each entity to journey to the mouse position (or modified mouse
	 * mouse position.
	 */
	for (size_t i = 0; i < no_players; i ++) {
		JourneyComponentPtr journey = Entity_fetch_component(
			players[i], JOURNEY_COMPONENT
		);
		JourneyComponent_journey_to(journey, chosen_tile, chosen_x, chosen_y);
	}
}

void quick_sort(EntityPtr * draw_order, const int length) {
	if (length < 2) {
		return;
	}

	int no_partitions = 1;
	int partition_i[length];
	int partition_size[length];
	partition_i[0] = 0;
	partition_size[0] = length;

	while (no_partitions) {
		int left_i = partition_i[0];
		int pivot_i = left_i + partition_size[0] - 1;
		int right_i = pivot_i - 1;

		MappedComponentPtr p_val = (MappedComponentPtr)Entity_fetch_component(
			draw_order[pivot_i], MAPPED_COMPONENT
		);

		MappedComponentPtr l_val;
		MappedComponentPtr r_val;

		while (right_i > left_i) {
			l_val = (MappedComponentPtr)Entity_fetch_component(
				draw_order[left_i], MAPPED_COMPONENT
			);
			r_val = (MappedComponentPtr)Entity_fetch_component(
				draw_order[right_i], MAPPED_COMPONENT
		    );

			bool swap_left = l_val->y < p_val->y;
			if (!swap_left) {
				left_i ++;
			}

			bool swap_right = r_val->y > p_val->y;
			if (!swap_right) {
				right_i --;
			}

			if (swap_left && swap_right) {
				draw_order[left_i] = ((ComponentPtr)r_val)->entity;
				draw_order[right_i] = ((ComponentPtr)l_val)->entity;

				if (left_i + 1 == right_i) {
					break;
				}
				left_i ++;
				right_i --;
			}
		}

		int final_pos = -1;
		if (right_i == left_i) {
			if (((MappedComponentPtr)Entity_fetch_component(
				draw_order[left_i], MAPPED_COMPONENT
			))->y < p_val->y) {
				final_pos = left_i;
			}
			else {
				final_pos = left_i + 1;
			}
		}
		else {
			final_pos = right_i;
		}

		draw_order[pivot_i] = draw_order[final_pos];
		draw_order[final_pos] = ((ComponentPtr)p_val)->entity;

		if (final_pos > partition_i[0] + 1) {
			no_partitions ++;
			partition_i[no_partitions - 1] = partition_i[0];
			partition_size[no_partitions - 1] = final_pos - partition_i[0];
		}

		if (final_pos < pivot_i - 1) {
			no_partitions ++;
			partition_i[no_partitions - 1] = final_pos + 1;
			partition_size[no_partitions - 1] = pivot_i - final_pos;
		}

		no_partitions --;
		if (no_partitions) {
			for (int i = 1; i <= no_partitions; i ++) {
				partition_i[i - 1] = partition_i[i];
				partition_size[i - 1] = partition_size[i];
			}
		}
	}
}

RoomPtr Game_find_room(GamePtr me, const char * name) {
	for (size_t i = 0; i < me->no_rooms; i ++) {
		RoomPtr room = me->rooms[i];
		if (strcmp(name, room->name) == 0) {
			return room;
		}
	}
	printf("Error. Room '%s' not found!\n", name);
	exit(-1);
}

EntityPtr Game_find_entity(GamePtr me, char * name) {
	for (size_t i = 0; i < me->sys->no_entities; i ++) {
		EntityPtr entity = me->sys->entities[i];
		if (strcmp(name, entity->id) == 0) {
			return entity;
		}
	}
	printf("Error. Entity '%s' not found!\n", name);
	exit(-1);
}
