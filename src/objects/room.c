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

#include <math.h>
#include <stdlib.h>

#include "./room.h"
#include "../solver.h"

void Room_init(
	RoomPtr me, const char * name, int focal_point_height, float scale_rate,
	int tiles_x
) {
	me->name = (const char *) malloc(sizeof(const char));
	me->name = name;
	me->planes = (PlanePtr *) malloc(sizeof(Plane));
	me->no_planes = 0;
	me->focal_point_height = focal_point_height;
	me->scale_rate = scale_rate;
	me->tiles_x = tiles_x;
}

PlanePtr Room_add_plane(
	RoomPtr me, int min_x, int min_y, int max_x, int max_y, int h, int h_x,
	int h_y
) {
	me->no_planes ++;
	me->planes = (PlanePtr *) realloc(
		me->planes, me->no_planes * sizeof(PlanePtr)
	);

	me->planes[me->no_planes - 1] = (PlanePtr) malloc(sizeof(Plane));
	PlanePtr plane = me->planes[me->no_planes - 1];
	Plane_init(plane, me, min_x, min_y, max_x, max_y, h, h_x, h_y);

	return plane;
}

int Room_get_point_x(RoomPtr me, ScreenPtr screen, double x, double y) {
	return screen->width / 2 + (x - me->tiles_x / 2) * (
		screen->width / me->tiles_x
	) * pow(1 - me->scale_rate, y);
}

int Room_get_point_y(
	RoomPtr me, ScreenPtr screen, double x, double y, double z
) {
	return screen->height - z - (
		screen->height - z - me->focal_point_height
	) * (1 - pow(1 - me->scale_rate, y));
}

int Room_no_tiles(RoomPtr me) {
	int no_all_tiles = 0;

	for (int plane_i = 0; plane_i < me->no_planes; plane_i ++) {
		no_all_tiles += Plane_no_tiles(me->planes[plane_i]);
	}

	return no_all_tiles;
}

void Room_join_tiles(RoomPtr me, TilePtr tile_1, TilePtr tile_2) {
	if (tile_1->x != tile_2->x && tile_1->y != tile_2->y) {
		printf(
			"(%d, %d) and (%d, %d) cannot be neighbours!", tile_1->x,
			tile_1->y, tile_2->x, tile_2->y
		);
		exit(-1);
	}
	else if (
		!(abs(tile_1->x - tile_2->x) == 1 || abs(tile_1->y - tile_2->y) == 1)
	) {
		printf(
			"(%d, %d) and (%d, %d) cannot be neighbours!", tile_1->x,
			tile_1->y, tile_2->x, tile_2->y
		);
		exit(-1);
	}

	if (tile_1->x > tile_2->x) {
		tile_1->neighbours[0] = tile_2;
		tile_2->neighbours[2] = tile_1;
	}
	else if (tile_1->x < tile_2->x) {
		tile_1->neighbours[2] = tile_2;
		tile_2->neighbours[0] = tile_1;
	}
	else if (tile_1->y > tile_2->y) {
		tile_1->neighbours[1] = tile_2;
		tile_2->neighbours[3] = tile_1;
	}
	else if (tile_1->y < tile_2->y) {
		tile_1->neighbours[3] = tile_2;
		tile_2->neighbours[1] = tile_1;
	}
}

void Plane_init(
	PlanePtr me, RoomPtr room, int min_x, int min_y, int max_x, int max_y,
	int h, int h_x, int h_y
) {
	me->room = room;
	me->min_x = min_x;
	me->min_y = min_y;
	me->max_x = max_x;
	me->max_y = max_y;
	me->h = h;
	me->h_x = h_x;
	me->h_y = h_y;
	me->tiles = (TilePtr *) malloc(Plane_no_tiles(me) * sizeof(TilePtr));
	me->tile_edges = (TileEdgePtr *) malloc((
		2 * Plane_width(me) * Plane_height(me) + Plane_width(me)
		+ Plane_height(me)
	) * sizeof(TileEdgePtr));

	int no_tile = 0;
	for (int i = min_x; i < max_x; i ++) {
		for (int j = min_y; j < max_y; j ++) {
			me->tiles[no_tile] = (TilePtr) malloc(sizeof(Tile));
			Tile_init(me->tiles[no_tile], me, i, j, true);
			no_tile ++;
		}
	}

	for (int i = 0; i < no_tile; i ++) {
		Tile_assign_neighbours(me->tiles[i]);
	}

	int no_tile_edge = 0;
	for (int i = min_x; i <= max_x; i ++) {
		for (int j = min_y; j <= max_y; j ++) {
			if (i < max_x) {
				me->tile_edges[no_tile_edge] = (TileEdgePtr) malloc(
					sizeof(TileEdge)
				);
				TileEdge_init(me->tile_edges[no_tile_edge], me, i, i + 1, j, j);
				no_tile_edge ++;
			}
			if (j < max_y) {
				me->tile_edges[no_tile_edge] = (TileEdgePtr) malloc(
					sizeof(TileEdge)
				);
				TileEdge_init(me->tile_edges[no_tile_edge], me, i, i, j, j + 1);
				no_tile_edge ++;
			}
		}
	}
}

TilePtr Plane_get_tile(PlanePtr me, int x, int y) {
	return me->tiles[
	    Plane_rel_x(me, x) * Plane_height(me) + Plane_rel_y(me, y)
	];
}

TileEdgePtr Plane_get_tile_edge(PlanePtr me, int x1, int x2, int y1, int y2) {
	if (x1 == x2) {
		if (x1 < me->max_x) {
			return me->tile_edges[
			    (2 * Plane_height(me) + 1) * Plane_rel_x(me, x1)
			    + 1 + 2 * Plane_rel_y(me, y1)
			];
		}
		else {
			return me->tile_edges[
				(2 * Plane_height(me) + 1) * Plane_width(me)
				+ Plane_rel_y(me, y1)
			];
		}
	}
	else {
		return me->tile_edges[
		    (2 * Plane_width(me) + 1) * Plane_rel_x(me, x1)
		    + 2 * Plane_rel_y(me, y1)
		];
	}
}

int Plane_rel_x(PlanePtr me, int x) {
	return x - me->min_x;
}

int Plane_rel_y(PlanePtr me, int y) {
	return y - me->min_y;
}

int Plane_width(PlanePtr me) {
	return Plane_rel_x(me, me->max_x);
}

int Plane_height(PlanePtr me) {
	return Plane_rel_y(me, me->max_y);
}

void Plane_draw(PlanePtr me, ScreenPtr screen) {
    SDL_SetRenderDrawColor(screen->rend, 255, 255, 0, 255);

    for (int i = 0; i < Plane_no_tile_edges(me); i ++) {
    	TileEdgePtr edge = me->tile_edges[i];
    	if (
			(edge->tile1 != NULL && edge->tile1->walkable)
			|| (edge->tile2 != NULL && edge->tile2->walkable)
		) {
        	SDL_RenderDrawLine(
        		screen->rend,
        		Room_get_point_x(me->room, screen, edge->x1, edge->y1),
        		Room_get_point_y(
    				me->room, screen, edge->x1, edge->y1,
    				Plane_get_height(me, edge->x1, edge->y1)
    			),
        		Room_get_point_x(me->room, screen, edge->x2, edge->y2),
        		Room_get_point_y(
    				me->room, screen, edge->x2, edge->y2,
    				Plane_get_height(me, edge->x2, edge->y2)
    			)
        	);
    	}
    }
}

double Plane_get_height(PlanePtr me, double x, double y) {
	return me->h + me->h_x * x + me->h_y * y;
}

int Plane_no_tiles(PlanePtr me) {
	return Plane_height(me) * Plane_width(me);
}

int Plane_no_tile_edges(PlanePtr me) {
	return (2 * Plane_height(me) + 1) * Plane_width(me) + Plane_height(me);
}

double Plane_get_mapped_x(
	PlanePtr me, ScreenPtr screen, int mouse_x, double coord_y
) {
	return (
		((double)me->room->tiles_x / screen->width)
		* pow(1 - me->room->scale_rate, -coord_y)
		* (mouse_x - screen->width / 2)
		+ (double)me->room->tiles_x / 2
	);
}

double Plane_get_mapped_y(
	PlanePtr me, ScreenPtr screen, int mouse_x, int mouse_y, int * rc
) {
	double result = calculate_y(
		mouse_y, me->room->focal_point_height, me->h_x, me->room->tiles_x,
		mouse_x, screen->width, screen->height, me->h, me->h_y,
		me->room->scale_rate, me->min_y, me->max_y, rc
	);

	return result;
}

void Tile_init(TilePtr me, PlanePtr plane, int x, int y, bool walkable) {
	me->plane = plane;
	me->x = x;
	me->y = y;
	me->walkable = walkable;
}

void Tile_assign_neighbours(TilePtr me) {
	/**
	 * Finds the addresses to each of the neighbours, if they exist or will
	 * exist.
	 */
	if (me->x > me->plane->min_x) {
		me->neighbours[0] = Plane_get_tile(me->plane, me->x - 1, me->y);
	}
	else {
		me->neighbours[0] = NULL;
	}

	if (me->y > me->plane->min_y) {
		me->neighbours[1] = Plane_get_tile(me->plane, me->x, me->y - 1);
	}
	else {
		me->neighbours[1] = NULL;
	}

	if (me->x < me->plane->max_x - 1) {
		me->neighbours[2] = Plane_get_tile(me->plane, me->x + 1, me->y);
	}
	else {
		me->neighbours[2] = NULL;
	}

	if (me->y < me->plane->max_y - 1) {
		me->neighbours[3] = Plane_get_tile(me->plane, me->x, me->y + 1);
	}
	else {
		me->neighbours[3] = NULL;
	}
}

void Tile_set_walkable(TilePtr me, bool walkable) {
	me->walkable = walkable;
}

bool Tile_contains_point(TilePtr me, double x, double y) {
	return me->x <= x && x <= me->x + 1 && me->y <= y && y <= me->y + 1;
}

void TileEdge_init(
	TileEdgePtr me, PlanePtr plane, int x1, int x2, int y1, int y2
) {
	me->plane = plane;
	me->x1 = x1;
	me->x2 = x2;
	me->y1 = y1;
	me->y2 = y2;

	if (x1 == x2) {
		if (x1 > plane->min_x) {
			me->tile1 = Plane_get_tile(plane, x1 - 1, y1);
		}
		else {
			me->tile1 = NULL;
		}
		if (x1 < plane->max_x) {
			me->tile2 = Plane_get_tile(plane, x1, y1);
		}
		else {
			me->tile2 = NULL;
		}
	}

	if (y1 == y2) {
		if (y1 > plane->min_y) {
			me->tile1 = Plane_get_tile(plane, x1, y1 - 1);
		}
		else {
			me->tile1 = NULL;
		}
		if (y1 < plane->max_y) {
			me->tile2 = Plane_get_tile(plane, x1, y1);
		}
		else {
			me->tile2 = NULL;
		}
	}
}
