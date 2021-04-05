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
