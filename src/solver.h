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

#ifndef __SOLVER_H__
#define __SOLVER_H__

double calculate_y(
	int display_y,
	int focal_point_height,
	int h_x,
	int tile_x,
	int display_x,
	int screen_width,
	int screen_height,
	int absolute_height,
	int h_y,
	double scale,
	int lower_bound,
	int upper_bound,
	int * rc
);

double y_equation(
	int display_y,
	int focal_point_height,
	int h_x,
	int tile_x,
	int display_x,
	int screen_width,
	int screen_height,
	int absolute_height,
	int h_y,
	double scale,
	double y_estimate
);

#endif
