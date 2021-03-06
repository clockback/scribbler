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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "./solver.h"

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
) {
	double lower_y = (double)lower_bound;
	double upper_y = (double)upper_bound;

	double lower_f = y_equation(
		display_y, focal_point_height, h_x, tile_x, display_x, screen_width,
		screen_height, absolute_height, h_y, scale, lower_y
	);
	double upper_f = y_equation(
		display_y, focal_point_height, h_x, tile_x, display_x, screen_width,
		screen_height, absolute_height, h_y, scale, upper_y
	);

	double tol = 0.01;

	if ((lower_f > 0) == (upper_f > 0)) {
		*rc = -1;
		return 0;
	}

	*rc = 0;

	while (fabs(lower_f - upper_f) > tol) {
		double new_y = (upper_y + lower_y) / 2;
		double new_f = y_equation(
			display_y, focal_point_height, h_x, tile_x, display_x, screen_width,
			screen_height, absolute_height, h_y, scale, new_y
		);
		if (new_f == 0) {
			return new_y;
		}

		if ((new_f > 0) == (lower_f > 0)) {
			lower_y = new_y;
			lower_f = new_f;
		}
		else {
			upper_y = new_y;
			upper_f = new_f;
		}
	}

	return (upper_y + lower_y) / 2;
}

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
) {
	return (
		display_y - focal_point_height
		+ (h_x * tile_x * display_x) / screen_width
		- (h_x * tile_x) / 2 - (
			screen_height - absolute_height - (h_x * tile_x) / 2
			- h_y * y_estimate - focal_point_height
		) * pow(1 - scale, y_estimate)
	);
}
