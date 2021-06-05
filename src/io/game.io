# Creates the initial room.
home_lounge := Create room("Home Lounge") set_focal_point(-200) \
	set_scale_rate(0.05) set_tile_x(10) select;

# Creates the ground floor of the home lounge.
ground_floor := Create plane("Ground Floor", home_lounge) set_y_bounds(0, 5) \
	set_unwalkable(C(9, 1), C(9, 2), C(9, 3), C(9, 4));

# Creates a ramp to connect the ground floor and upper floor.
ramp := Create plane("Ramp", home_lounge) set_x_bounds(9, 10) \
	set_y_bounds(1, 4) set_base_height(-35) set_height_slope_y(35);

# Creates an upper floor above the lounge.
upper_floor := Create plane("Upper Floor", home_lounge) set_y_bounds(0, 5) \
	set_base_height(105) set_unwalkable(C(9, 0), C(9, 1), C(9, 2), C(9, 3));

# Connects the tiles which need to be connected.
home_lounge connect_tiles(
	list(ground_floor, C(9, 0), ramp, C(9, 1)), \
	list(upper_floor, C(9, 4), ramp, C(9, 3)) \
);

micah := Create entity("Micah", "player", "world");
micah add_mapped(1.5, 0.5, ground_floor);
micah add_move;
micah add_journey(home_lounge);
micah add_animate("assets/images/characters/micah") \
	enable_scaling(1) \
	load_sprite("STAND_FORWARDS", "front.png") \
	load_sprite("STAND_BACKWARDS", "back.png") \
	load_sprite("STAND_LEFT", "left.png") \
	load_sprite("STAND_RIGHT", "right.png") \
	load_sprite("WALK_FORWARDS_1", "front_walk_1.png") \
	load_sprite("WALK_FORWARDS_2", "front_walk_2.png") \
	load_sprite("WALK_FORWARDS_3", "front_walk_3.png") \
	load_sprite("WALK_FORWARDS_4", "front_walk_4.png") \
	load_sprite("WALK_FORWARDS_5", "front_walk_5.png") \
	load_sprite("WALK_FORWARDS_6", "front_walk_6.png") \
	load_sprite("WALK_BACKWARDS_1", "back_walk_1.png") \
	load_sprite("WALK_BACKWARDS_2", "back_walk_2.png") \
	load_sprite("WALK_BACKWARDS_3", "back_walk_3.png") \
	load_sprite("WALK_BACKWARDS_4", "back_walk_4.png") \
	load_sprite("WALK_BACKWARDS_5", "back_walk_5.png") \
	load_sprite("WALK_BACKWARDS_6", "back_walk_6.png") \
	load_sprite("WALK_LEFT_1", "left_walk_1.png") \
	load_sprite("WALK_LEFT_2", "left_walk_2.png") \
	load_sprite("WALK_LEFT_3", "left_walk_3.png") \
	load_sprite("WALK_LEFT_4", "left_walk_4.png") \
	load_sprite("WALK_LEFT_5", "left_walk_5.png") \
	load_sprite("WALK_LEFT_6", "left_walk_6.png") \
	load_sprite("WALK_RIGHT_1", "right_walk_1.png") \
	load_sprite("WALK_RIGHT_2", "right_walk_2.png") \
	load_sprite("WALK_RIGHT_3", "right_walk_3.png") \
	load_sprite("WALK_RIGHT_4", "right_walk_4.png") \
	load_sprite("WALK_RIGHT_5", "right_walk_5.png") \
	load_sprite("WALK_RIGHT_6", "right_walk_6.png") \
	use_sprite("STAND_FORWARDS");
micah add_speak;

rubbish_bin := Create entity("Rubbish bin", "world");
rubbish_bin add_mapped(0.4, 0.0, ground_floor);
rubbish_bin add_sprite("assets/images/scenery/park/rubbish_bin.png") \
	enable_scaling(1);
rubbish_bin add_click \
	set_interact_point(C(1, 0, ground_floor), 1, 0.1, "LEFT_DIR");
rubbish_bin add_interact("Rubbish bin");
ground_floor set_unwalkable(C(0, 0));

observe_bin_scenario := Create scenario;
observe_bin_scenario add_trigger interact_entity(rubbish_bin);
observe_bin_scenario add_condition numeric_equal_to(entity_get_x(micah), 1);
observe_bin_scenario add_action set_user_input(0);
observe_bin_scenario add_action wait(200);
observe_bin_scenario add_action \
	entity_journey_to(micah, 2.3, 1.3, ground_floor);
observe_bin_scenario add_action speak(micah, "Hello", 100);
observe_bin_scenario add_action set_user_input(1);
