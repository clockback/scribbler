#include <stdarg.h>

#include "JourneyComponent.h"
#include "ECS.h"

ComponentPtr JourneyComponent_init(
	void * me_void, EntityPtr entity, va_list * args
) {
	JourneyComponentPtr me = (JourneyComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, JOURNEY_COMPONENT);

	me->mapped = (MappedComponentPtr)Entity_fetch_component(
		entity, MAPPED_COMPONENT
	);
	me->move = (MoveComponentPtr)Entity_fetch_component(
		entity, MOVE_COMPONENT
	);

	me->target_tile = NULL;
	me->target_x = 0.0f;
	me->target_y = 0.0f;
	me->room = va_arg(*args, RoomPtr);
	me->path_length = 0;
	me->path_tiles = (TilePtr *) malloc(sizeof(TilePtr));
	me->path_planes = (PlanePtr *) malloc(sizeof(TilePtr));

	return me_component;
}

void JourneyComponent_update(void * me_void) {
	JourneyComponentPtr me = (JourneyComponentPtr)me_void;

	if (me->path_length == 0) {
		MoveComponent_stop_moving(me->move);
		return;
	}

	if (me->path_length >= 2 && (
		Tile_contains_point(me->path_tiles[1], me->mapped->x, me->mapped->y)
		|| (me->path_length >= 3 && Tile_contains_point(
			me->path_tiles[2], me->mapped->x, me->mapped->y
		))
	)){
		int no_to_remove = 1;
		if (me->path_length >= 3 && Tile_contains_point(
			me->path_tiles[2], me->mapped->x, me->mapped->y
		)) {
			no_to_remove = 2;
		}

		for (int i = 0; i < me->path_length; i ++) {
			me->path_tiles[i] = me->path_tiles[i + no_to_remove];
			me->path_planes[i] = me->path_planes[i + no_to_remove];
		}

		me->path_length -= no_to_remove;

		me->path_tiles = (TilePtr *) realloc(
			me->path_tiles, me->path_length * sizeof(TilePtr)
		);
		me->path_planes = (PlanePtr *) realloc(
			me->path_planes, me->path_length * sizeof(PlanePtr)
		);
		me->mapped->plane = me->path_planes[0];
	}

	if (
		me->mapped->x == me->target_x && me->mapped->y == me->target_y
		&& me->mapped->plane == me->target_tile->plane
	) {
		me->path_length = 0;
		MoveComponent_stop_moving(me->move);
		return;
	}

	double max_speed = 0.02f;

	TilePtr current = me->path_tiles[0];
	TilePtr next = me->path_tiles[1];

	if (me->path_length == 1) {
		double dx = me->target_x - me->mapped->x;
		double dy = me->target_y - me->mapped->y;
		double total = fabs(dx) + fabs(dy);
		me->move->vx = max_speed * dx / total;
		me->move->vy = max_speed * dy / total;

		if (
			(me->mapped->x + me->move->vx > me->target_x)
			!= (me->mapped->x > me->target_x)
		) {
			me->move->vx = dx;
		}

		if (
			(me->mapped->y + me->move->vy > me->target_y)
			!= (me->mapped->y > me->target_y)
		) {
			me->move->vy = dy;
		}

		return;
	}

	bool start_horizontal = current->y == next->y;

	int j = 0;
	for (int i = 2; i < me->path_length; i ++) {
		if (
				(me->path_tiles[i]->y != current->y && start_horizontal)
				|| (me->path_tiles[i]->x != current->x && !start_horizontal)
		) {
			j = i;
			break;
		}
	}

	double dx;
	double dy;
	if (j) {
		if (me->path_tiles[j - 2]->x > me->path_tiles[j]->x) {
			dx = me->path_tiles[j - 2]->x - me->mapped->x;
		}
		else {
			dx = me->path_tiles[j]->x - me->mapped->x;
		}
		if (me->path_tiles[j - 2]->y > me->path_tiles[j]->y) {
			dy = me->path_tiles[j - 2]->y - me->mapped->y;
		}
		else {
			dy = me->path_tiles[j]->y - me->mapped->y;
		}
	}
	else {
		dx = me->target_x - me->mapped->x;
		dy = me->target_y - me->mapped->y;
	}

	double total = fabs(dx) + fabs(dy);
	me->move->vx = max_speed * dx / total;
	me->move->vy = max_speed * dy / total;

	if (
		j == 0 && (me->mapped->x + me->move->vx > me->target_x)
		!= (me->mapped->x > me->target_x)
	) {
		me->move->vx = dx;
	}

	if (
		j == 0 && (me->mapped->y + me->move->vy > me->target_y)
		!= (me->mapped->y > me->target_y)
	) {
		me->move->vy = dy;
	}
}

void JourneyComponent_draw(void * me_void) {
}

void JourneyComponent_delete(void * me_void) {
}

void JourneyComponent_journey_to(
	JourneyComponentPtr me, TilePtr tile, double x, double y
) {
	me->target_tile = tile;
	me->target_x = x;
	me->target_y = y;

	JourneyComponent_dijkstra(me);
}

void JourneyComponent_dijkstra(JourneyComponentPtr me) {
	/* Finds the tile which the entity presently occupies. */
	TilePtr current_tile = Plane_get_tile(
		me->mapped->plane, (int)me->mapped->x, (int)me->mapped->y
	);

	/* Finds the tile which is being targeted. */
	TilePtr aim = Plane_get_tile(
		me->target_tile->plane, me->target_x, me->target_y
	);

	/* Finds the number of tiles in the room. */
	int no_all_tiles = Room_no_tiles(me->room);

	/**
	 * Allocates an amount of memory to the walkable tiles equivalent to the
	 * the number of tiles both walkable and unwalkable.
	 */
	TilePtr * unreached = (TilePtr *) malloc(no_all_tiles * sizeof(TilePtr));

	/**
	 * Finds the number of walkable tiles in the room (besides that which the
	 * entity already occupies).
	 */
	int no_unreached = 0;
	for (int plane_i = 0; plane_i < me->room->no_planes; plane_i ++) {
		PlanePtr plane = me->room->planes[plane_i];
		for (int x = plane->min_x; x < plane->max_x; x ++) {
			for (int y = plane->min_y; y < plane->max_y; y ++) {
				TilePtr tile = Plane_get_tile(plane, x, y);
				if (tile->walkable && tile != current_tile) {
					unreached[no_unreached] = tile;
					no_unreached ++;
				}
			}
		}
	}

	/**
	 * Resizes the array of tiles according to how many tiles are walkable and
	 * not occupied.
	 */
	unreached = (TilePtr *) realloc(unreached, no_unreached * sizeof(TilePtr));
	TilePtr * reached = (TilePtr *) malloc(
		(no_unreached + 1) * sizeof(TilePtr)
	);
	int no_reached = 1;
	reached[0] = current_tile;

	// Initializes best paths to each of the tiles in the system.
	double * costs = (double *) malloc((no_unreached + 1) * sizeof(double));
	int * lengths = (int *) malloc((no_unreached + 1) * sizeof(int));
	TilePtr * ancestors = (TilePtr *) malloc(
		(no_unreached + 1) * sizeof(TilePtr)
	);
	int * ancestors_i = (int *) malloc(
		(no_unreached + 1) * sizeof(int)
	);

	/* Initializes incomplete paths for every tile not yet reached. */
	for (int i = 1; i < no_reached + no_unreached; i ++) {
		costs[i] = 999999;
		lengths[0] = 0;
		ancestors[i] = NULL;
		ancestors_i[i] = 0;
	}

	/* Initializes the first tile (the only one reached). */
	costs[0] = 0;
	lengths[0] = 0;
	ancestors[0] = NULL;
	ancestors_i[0] = -1;

	/* Acknowledges that the index of the target has not yet been found. */
	int target_idx = -1;

	/* Continues to iterate until the target tile has been reached. */
	while (target_idx == -1) {
		/* Initializes a non-existent "best" tile to have been reached. */
		TilePtr best_ancestor = NULL;
		TilePtr best_tile = NULL;
		double best_cost = 999999.0f;
		int best_ancestor_i = 0;

		/* Iterates over all the reached tiles in the system. */
		for (int i = 0; i < no_reached; i ++) {
			/* Finds the particular tile which has been reached.*/
			TilePtr reached_tile = reached[i];
			double cost_so_far = costs[i];

			/* Investigates each of the tile's neighbouring tiles. */
			for (int j = 0; j < 4; j ++) {
				/* Finds the individual neighbour tile. */
				TilePtr neighbour = reached_tile->neighbours[j];

				/**
				 * Discards the neighbour if it does not exist or cannot be
				 * walked upon.
				 */
				if (neighbour == NULL || !neighbour->walkable) {
					continue;
				}

				/* Ignores the neighbour if it has already been reached. */
				bool already_found = false;
				for (int k = 0; k < no_reached; k ++) {
					if (reached[k] == neighbour) {
						already_found = true;
						break;
					}
				}
				if (already_found) {
					continue;
				}

				/* Finds the cost of travelling to the tile in this way. */
				double cost;
				if (i == 0) {
					cost = 1.0f;
				}
				else if (
					(j == 0 || j == 2) == (reached_tile->y == ancestors[i]->y)
				) {
					cost = 1.0f;
				}
				else {
					cost = 0.7071f;
				}

				/**
				 * If thereby accessing the neighbouring tile is the best path
				 * so far, updates the best path.
				 */
				if (cost_so_far + cost < best_cost) {
					best_tile = neighbour;
					best_ancestor = reached_tile;
					best_cost = cost_so_far + cost;
					best_ancestor_i = i;
				}
			}
		}

		/**
		 * If no tile was found that can be reached, stops searching
		 * altogether.
		 */
		if (best_tile == NULL) {
			break;
		}

		/**
		 * Adds the tile to the list of reached tiles, along with its
		 * associated information.
		 */
		reached[no_reached] = best_tile;
		costs[no_reached] = best_cost;
		ancestors[no_reached] = best_ancestor;
		ancestors_i[no_reached] = best_ancestor_i;
		lengths[no_reached] = lengths[best_ancestor_i] + 1;

		/**
		 * Remembers the index if it is the target tile.
		 */
		if (best_tile == aim) {
			target_idx = no_reached;
		}

		/* Records the new number of tiles that have been reached. */
		no_reached ++;

		/**
		 * Cleans up the array which previously contained the now found tile.
		 */
		for (int i = 0; i < no_unreached; i ++) {
			int found = 0;
			if (unreached[i] == best_tile) {
				found = 1;
			}
			else {
				unreached[i] = unreached[i + found];
			}
		}
		no_unreached --;

	}

	/* Acknowledges the length of the new path. */
	me->path_length = lengths[target_idx] + 1;
	me->path_tiles = (TilePtr *) realloc(
		me->path_tiles, me->path_length * sizeof(TilePtr)
	);
	me->path_planes = (PlanePtr *) realloc(
		me->path_planes, me->path_length * sizeof(PlanePtr)
	);

	/* Stores the target tile as the last in the path. */
	me->path_tiles[me->path_length - 1] = aim;
	me->path_planes[me->path_length - 1] = aim->plane;

	/* Remembers the most recently considered tile. */
	int idx = target_idx;

	/**
	 * Iterates backwards through the list of tiles until a path has been
	 * constructed.
	 */
	for (int i = me->path_length - 2; i >= 0; i --) {
		me->path_tiles[i] = ancestors[idx];
		me->path_planes[i] = ancestors[idx]->plane;
		idx = ancestors_i[idx];
	}

	/* Releases memory no longer needed. */
	free(reached);
	free(unreached);
	free(costs);
	free(lengths);
	free(ancestors);
	free(ancestors_i);
}
