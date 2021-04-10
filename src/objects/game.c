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
#include "../ECS/Components.h"

void Game_init(GamePtr me, int screen_width, int screen_height) {
	me->running = true;

	me->screen = (ScreenPtr) malloc(sizeof(Screen));
    Screen_init(me->screen, screen_width, screen_height, 3);

    me->rend = me->screen->rend;

	me->sys = (SystemPtr) malloc(sizeof(System));
    System_init(me->sys, me->screen);

    me->event = (SDL_Event *) malloc(sizeof(SDL_Event));

    me->room = (RoomPtr) malloc(sizeof(Room));

    Game_prepare(me);
}

void Game_prepare(GamePtr me) {
    // Creates a room.
    Room_init(me->room, "Home Lounge", -200, 0.05, 10);
    PlanePtr ground_floor = Room_add_plane(me->room, 0, 0, 10, 5, 0, 0, 0);
    Tile_set_walkable(Plane_get_tile(ground_floor, 9, 1), false);
    Tile_set_walkable(Plane_get_tile(ground_floor, 9, 2), false);
    Tile_set_walkable(Plane_get_tile(ground_floor, 9, 3), false);
    Tile_set_walkable(Plane_get_tile(ground_floor, 9, 4), false);
    PlanePtr ramp = Room_add_plane(me->room, 9, 1, 10, 4, -35, 0, 35);
    PlanePtr upper_floor = Room_add_plane(me->room, 0, 0, 10, 5, 105, 0, 0);
    Tile_set_walkable(Plane_get_tile(upper_floor, 9, 0), false);
    Tile_set_walkable(Plane_get_tile(upper_floor, 9, 1), false);
    Tile_set_walkable(Plane_get_tile(upper_floor, 9, 2), false);
    Tile_set_walkable(Plane_get_tile(upper_floor, 9, 3), false);

    Room_join_tiles(
		me->room, Plane_get_tile(ground_floor, 9, 0),
		Plane_get_tile(ramp, 9, 1)
	);

    Room_join_tiles(
		me->room, Plane_get_tile(upper_floor, 9, 4),
		Plane_get_tile(ramp, 9, 3)
	);

    // Creates a player entity.
    EntityPtr player = System_add_entity(me->sys, "Micah");
    Entity_add_group(player, GROUP_PLAYERS);
    Entity_add_group(player, GROUP_WORLD);
    Entity_add_component(
		player, MAPPED_COMPONENT, 3, 1.5f, 0.5f, ground_floor
	);
    Entity_add_component(player, MOVE_COMPONENT, 0);
    Entity_add_component(player, JOURNEY_COMPONENT, 1, me->room);
    AnimateComponentPtr animate = Entity_add_component(
		player, ANIMATE_COMPONENT, 1, "assets/images/characters/micah"
	);

    AnimateComponent_load_sprite(animate, STAND_FORWARDS, "front.png");
    AnimateComponent_load_sprite(animate, STAND_BACKWARDS, "back.png");
    AnimateComponent_load_sprite(animate, STAND_LEFT, "left.png");
    AnimateComponent_load_sprite(animate, STAND_RIGHT, "right.png");
    AnimateComponent_load_sprite(
		animate, WALK_FORWARDS_1, "front_walk_1.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_FORWARDS_2, "front_walk_2.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_FORWARDS_3, "front_walk_3.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_FORWARDS_4, "front_walk_4.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_FORWARDS_5, "front_walk_5.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_FORWARDS_6, "front_walk_6.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_BACKWARDS_1, "back_walk_1.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_BACKWARDS_2, "back_walk_2.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_BACKWARDS_3, "back_walk_3.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_BACKWARDS_4, "back_walk_4.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_BACKWARDS_5, "back_walk_5.png"
	);
    AnimateComponent_load_sprite(
		animate, WALK_BACKWARDS_6, "back_walk_6.png"
	);
    AnimateComponent_load_sprite(animate, WALK_LEFT_1, "left_walk_1.png");
    AnimateComponent_load_sprite(animate, WALK_LEFT_2, "left_walk_2.png");
    AnimateComponent_load_sprite(animate, WALK_LEFT_3, "left_walk_3.png");
    AnimateComponent_load_sprite(animate, WALK_LEFT_4, "left_walk_4.png");
    AnimateComponent_load_sprite(animate, WALK_LEFT_5, "left_walk_5.png");
    AnimateComponent_load_sprite(animate, WALK_LEFT_6, "left_walk_6.png");
    AnimateComponent_load_sprite(animate, WALK_RIGHT_1, "right_walk_1.png");
    AnimateComponent_load_sprite(animate, WALK_RIGHT_2, "right_walk_2.png");
    AnimateComponent_load_sprite(animate, WALK_RIGHT_3, "right_walk_3.png");
    AnimateComponent_load_sprite(animate, WALK_RIGHT_4, "right_walk_4.png");
    AnimateComponent_load_sprite(animate, WALK_RIGHT_5, "right_walk_5.png");
    AnimateComponent_load_sprite(animate, WALK_RIGHT_6, "right_walk_6.png");
    AnimateComponent_use_sprite(animate, STAND_FORWARDS);

    // Creates a player entity.
    EntityPtr bin = System_add_entity(me->sys, "rubbish bin");
    Entity_add_group(bin, GROUP_WORLD);
    Entity_add_component(bin, MAPPED_COMPONENT, 3, 0.4f, 0.0f, ground_floor);
    SpriteComponentPtr sprite = (SpriteComponentPtr)Entity_add_component(
		bin, SPRITE_COMPONENT, 2,
		"assets/images/scenery/park/rubbish_bin.png", 1
	);
    SpriteComponent_enable_scaling(sprite, true);
    ClickComponentPtr click = Entity_add_component(bin, CLICK_COMPONENT, 0);
    ClickComponent_set_interact_point(
		click, Plane_get_tile(ground_floor, 1, 0), 1.0, 0.1, LEFT_DIR
    );
    Tile_set_walkable(Plane_get_tile(ground_floor, 0, 0), false);
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

	int mouse_x = me->event->motion.x / me->screen->scale;
	int mouse_y = me->event->motion.y / me->screen->scale;

	EntityPtr * world_entities = System_get_group(me->sys, GROUP_WORLD);
	size_t no_world_entities = System_get_group_size(me->sys, GROUP_WORLD);
	EntityPtr * draw_order = (EntityPtr *) malloc(
		no_world_entities * sizeof(EntityPtr)
	);
	for (size_t i = 0; i < no_world_entities; i ++) {
		draw_order[i] = world_entities[i];
	}

	quick_sort(draw_order, no_world_entities);

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

	EntityPtr * players = System_get_group(me->sys, GROUP_PLAYERS);
	size_t no_players = System_get_group_size(me->sys, GROUP_PLAYERS);

	int no_plane = -1;
	double chosen_x, chosen_y;
	double chosen_height = -999999;
	TilePtr chosen_tile = NULL;
	for (int i = 0; i < me->room->no_planes; i ++) {
		PlanePtr plane = me->room->planes[i];
    	double y = Plane_get_mapped_y(
			plane, me->screen, mouse_x, mouse_y, &rc
		);
    	if (rc != 0) {
    		continue;
    	}
		double x = Plane_get_mapped_x(plane, me->screen, mouse_x, y);
		if (x < plane->min_x || x > plane->max_x) {
			continue;
		}

		TilePtr tile = Plane_get_tile(plane, (int)x, (int)y);

		if (!tile->walkable) {
			double close_x = pow(x - (int)x - 0.5, 2);
			double close_y = pow(y - (int)y - 0.5, 2);
			double other_x;
			double other_y;
			if (close_x >= close_y) {
				other_y = y;
				if (x - (int)x <= 0.5 && (int)x > plane->min_x) {
					other_x = (double)(int)x;
					tile = Plane_get_tile(plane, (int)x - 1, (int)y);
				}
				else if (x - (int)x > 0.5 && (int)x < plane->max_x) {
					other_x = (double)(int)x + 1;
					tile = Plane_get_tile(plane, (int)x + 1, (int)y);
				}
				else {
					continue;
				}
			}
			else {
				other_x = x;
				if (y - (int)y <= 0.5 && (int)y > plane->min_y) {
					other_y = (double)(int)y;
					tile = Plane_get_tile(plane, (int)x, (int)y - 1);
				}
				else if (y - (int)y > 0.5 && (int)y < plane->max_y) {
					other_y = (double)(int)y + 1;
					tile = Plane_get_tile(plane, (int)x, (int)y + 1);
				}
				else {
					continue;
				}
			}

			if (tile->walkable) {
				x = other_x;
				y = other_y;
			}
			else {
				continue;
			}
		}

		double height = Plane_get_height(plane, x, y);
		if (height <= chosen_height) {
			continue;
		}

		no_plane = i;
		chosen_x = x;
		chosen_y = y;
		chosen_height = height;
		chosen_tile = tile;
	}

	if (no_plane == -1) {
		return;
	}

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
