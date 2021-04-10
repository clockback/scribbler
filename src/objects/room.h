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

#ifndef __ROOM_H__
#define __ROOM_H__

#include <stdbool.h>

#include "./graphics.h"

typedef struct Room Room;
typedef struct Plane Plane;
typedef struct Tile Tile;
typedef struct TileEdge TileEdge;

typedef Room * RoomPtr;
typedef Plane * PlanePtr;
typedef Tile * TilePtr;
typedef TileEdge * TileEdgePtr;

struct Room {
    const char * name;
    PlanePtr * planes;
    size_t no_planes;
    int focal_point_height;
    float scale_rate;
    int tiles_x;
};

void Room_init(
	RoomPtr me, const char * name, int focal_point_height, float scale_rate,
	int tiles_x
);
PlanePtr Room_add_plane(
	RoomPtr me, int min_x, int min_y, int max_x, int max_y, int h, int h_x,
	int h_y
);
int Room_get_point_x(RoomPtr me, ScreenPtr screen, double x, double y);
int Room_get_point_y(
	RoomPtr me, ScreenPtr screen, double x, double y, double z
);
int Room_no_tiles(RoomPtr me);
void Room_join_tiles(RoomPtr me, TilePtr tile_1, TilePtr tile_2);

struct Plane {
    RoomPtr room;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    int h;
    int h_x;
    int h_y;
    TilePtr * tiles;
    TileEdgePtr * tile_edges;
};

void Plane_init(
	PlanePtr me, RoomPtr room, int min_x, int min_y, int max_x, int max_y,
	int h, int h_x, int h_y
);
TilePtr Plane_get_tile(PlanePtr me, int x, int y);
TileEdgePtr Plane_get_tile_edge(PlanePtr me, int x1, int x2, int y1, int y2);
int Plane_rel_x(PlanePtr me, int x);
int Plane_rel_y(PlanePtr me, int y);
int Plane_width(PlanePtr me);
int Plane_height(PlanePtr me);
void Plane_draw(PlanePtr me, ScreenPtr screen);
double Plane_get_height(PlanePtr me, double x, double y);
int Plane_no_tiles(PlanePtr me);
int Plane_no_tile_edges(PlanePtr me);
double Plane_get_mapped_x(
	PlanePtr me, ScreenPtr screen, int mouse_x, double coord_y
);
double Plane_get_mapped_y(
	PlanePtr me, ScreenPtr screen, int mouse_x, int mouse_y, int * rc
);

struct Tile {
	PlanePtr plane;
	int x;
	int y;
	bool walkable;
	TilePtr neighbours[4];
};

void Tile_init(TilePtr me, PlanePtr plane, int x, int y, bool walkable);
void Tile_set_walkable(TilePtr me, bool walkable);
void Tile_assign_neighbours(TilePtr me);
bool Tile_contains_point(TilePtr me, double x, double y);

struct TileEdge {
    PlanePtr plane;
    int x1;
    int x2;
    int y1;
    int y2;
    TilePtr tile1;
    TilePtr tile2;
};

void TileEdge_init(
	TileEdgePtr me, PlanePtr plane, int x1, int x2, int y1, int y2
);

#endif
