#include "./game.h"
#include "./graphics.h"
#include "./room.h"
#include "../ECS/Components.h"

void Game_init(GamePtr me, int screen_width, int screen_height) {
	me->running = true;

	me->screen = (ScreenPtr) malloc(sizeof(Screen));
    Screen_init(me->screen, screen_width, screen_height);

    me->rend = me->screen->rend;

	me->sys = (SystemPtr) malloc(sizeof(System));
    System_init(me->sys, me->screen);

    me->event = (SDL_Event *) malloc(sizeof(SDL_Event));

    me->room = (RoomPtr) malloc(sizeof(Room));

    Game_prepare(me);
}

void Game_prepare(GamePtr me) {
    // Creates a room.
    Room_init(me->room, "Home Lounge", 0, 0.1, 10);
    PlanePtr ground_floor = Room_add_plane(me->room, 0, 0, 10, 5, 0, 0, 0);
    Tile_set_walkable(Plane_get_tile(ground_floor, 9, 1), false);
    Tile_set_walkable(Plane_get_tile(ground_floor, 9, 2), false);
    Tile_set_walkable(Plane_get_tile(ground_floor, 9, 3), false);
    Tile_set_walkable(Plane_get_tile(ground_floor, 9, 4), false);
    PlanePtr ramp = Room_add_plane(me->room, 9, 1, 10, 4, -100, 0, 100);
    PlanePtr upper_floor = Room_add_plane(me->room, 0, 0, 10, 5, 300, 0, 0);
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

    // Creates an entity.
    EntityPtr player = System_add_entity(me->sys);
    Entity_add_group(player, GROUP_PLAYERS);
    Entity_add_component(
		player, MAPPED_COMPONENT, 3, 6.5f, 2.5f, ground_floor
	);
    Entity_add_component(player, MOVE_COMPONENT, 0);
    Entity_add_component(player, JOURNEY_COMPONENT, 1, me->room);
    AnimateComponentPtr animate = Entity_add_component(
		player, ANIMATE_COMPONENT, 2, "assets/images/characters/micah", 3
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
}

void Game_handle_events(GamePtr me) {
    while (SDL_PollEvent(me->event)) {
        int rc;
        switch (me->event->type) {
        case SDL_QUIT:
            me->running = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
        	;
        	EntityPtr * players = System_get_group(me->sys, GROUP_PLAYERS);
        	size_t no_players = System_get_group_size(me->sys, GROUP_PLAYERS);

        	int no_plane = -1;
        	double chosen_x, chosen_y;
        	double chosen_height = -999999;
        	for (int i = 0; i < me->room->no_planes; i ++) {
            	double y = Plane_get_mapped_y(
    				me->room->planes[i], me->screen, me->event->motion.x,
    				me->event->motion.y, &rc
    			);
            	if (rc != 0) {
            		continue;
            	}
    			double x = Plane_get_mapped_x(
    				me->room->planes[i], me->screen, me->event->motion.x, y
    			);
    			if (
    				x < me->room->planes[i]->min_x
    				|| x > me->room->planes[i]->max_x
    			) {
    				continue;
    			}

    			if (!Plane_get_tile(
					me->room->planes[i], (int)x, (int)y
				)->walkable) {
    				continue;
    			}

    			double height = Plane_get_height(me->room->planes[i], x, y);
    			if (height <= chosen_height) {
    				continue;
    			}

    			no_plane = i;
    			chosen_x = x;
    			chosen_y = y;
    			chosen_height = height;
        	}

        	if (no_plane == -1) {
        		break;
        	}

        	for (size_t i = 0; i < no_players; i ++) {
    			JourneyComponentPtr journey = Entity_fetch_component(
					players[i], JOURNEY_COMPONENT
				);
        		JourneyComponent_journey_to(journey, Plane_get_tile(
					me->room->planes[no_plane], (int)chosen_x, (int)chosen_y
				), chosen_x, chosen_y);
        	}

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
    SDL_SetRenderDrawColor(me->rend, 255, 0, 0, 255);
    SDL_RenderClear(me->screen->rend);

	for (size_t i = 0; i < me->room->no_planes; i ++) {
		Plane_draw(me->room->planes[i], me->screen);
	}

	EntityPtr * players = System_get_group(me->sys, GROUP_PLAYERS);
	size_t no_players = System_get_group_size(me->sys, GROUP_PLAYERS);

	for (size_t i = 0; i < no_players; i ++) {
		Entity_draw(players[i]);
	}

    SDL_RenderPresent(me->screen->rend);
}

void Game_clean(GamePtr me) {
	SDL_DestroyWindow(me->screen->window);
	SDL_DestroyRenderer(me->rend);
	SDL_Quit();
}
