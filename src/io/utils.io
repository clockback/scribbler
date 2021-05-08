Create := Object clone;
Create planes := List clone;
Create rooms := List clone;
Create entities := List clone;

Get := Object clone;
Get planes := Create planes;
Get rooms := Create rooms;
Get entities := Create entities;

RoomObj := Object clone;
RoomObj name := "Unnamed";
RoomObj planes := List clone;
RoomObj focal_point := nil;
RoomObj scale_rate := nil;
RoomObj tile_x := nil;
RoomObj is_current := 0;
RoomObj tile_connections := List clone;

PlaneObj := Object clone;
PlaneObj name := "Unnamed";
PlaneObj room := nil;
PlaneObj min_x := nil;
PlaneObj min_y := nil;
PlaneObj max_x := nil;
PlaneObj max_y := nil;
PlaneObj h := nil;
PlaneObj hx := nil;
PlaneObj hy := nil;
PlaneObj unwalkable := List clone;

TileObj := Object clone;
TileObj x := 0;
TileObj y := 0;
TileObj plane := nil;

EntityObj := Object clone;
EntityObj name := "Unnamed";
EntityObj groups := List clone;
EntityObj mapped_c := nil;
EntityObj move_c := nil;
EntityObj sprite_c := nil;
EntityObj journey_c := nil;
EntityObj animate_c := nil;
EntityObj click_c := nil;
EntityObj interact_c := nil;

MappedObj := Object clone;
MappedObj x := 0;
MappedObj y := 0;
MappedObj plane := nil;

MoveObj := Object clone;

SpriteObj := Object clone;
SpriteObj img := nil;
SpriteObj scaling := nil;

JourneyObj := Object clone;
JourneyObj room := nil;

AnimateObj := Object clone;
AnimateObj path := nil;
AnimateObj scaling := nil;
AnimateObj images := nil;
AnimateObj current := nil;

ClickObj := Object clone;
ClickObj tile := nil;
ClickObj x := nil;
ClickObj y := nil;
ClickObj direction := nil;

InteractObj := Object clone;
InteractObj label := nil;

C := method(x, y, plane,
	tile := TileObj clone;
	tile x = x;
	tile y = y;
	tile plane = plane;
	tile;
);

Create room := method(name,
	new_room := RoomObj clone;
	rooms append(new_room);
	new_room name = name;
	new_room planes = List clone;
	new_room focal_point = 0;
	new_room scale_rate = 0;
	new_room tile_x = 12;
	new_room is_current = 0;

	new_room;
);

Create plane := method(name, room,
	new_plane := PlaneObj clone;
	planes append(new_plane);
	new_plane name = name;
	new_plane room = room;
	room planes append(new_plane);
	new_plane min_x = 0;
	new_plane min_y = 0;
	new_plane max_x = room tile_x;
	new_plane max_y = 6;
	new_plane h = 0;
	new_plane hx = 0;
	new_plane hy = 0;
	new_plane unwalkable = List clone;

	new_plane;
);

Create entity := method(name,
	new_entity := EntityObj clone;
	entities append(new_entity);
	new_entity name = name;
	new_entity groups = List clone;
	new_entity mapped_c = nil;
	new_entity move_c = nil;
	new_entity sprite_c = nil;
	new_entity journey_c = nil;
	new_entity animate_c = nil;
	new_entity click_c = nil;
	new_entity interact_c = nil;

	i := 1;
	group := call evalArgAt(i);

	while(group != nil,
		new_entity add_to_group(group);
		i = i + 1;
		group = call evalArgAt(i)
	);

	new_entity;
);

RoomObj set_focal_point := method(focal_point,
	self focal_point = focal_point;
	self;
);

RoomObj set_scale_rate := method(scale_rate,
	self scale_rate = scale_rate;
	self;
);

RoomObj set_tile_x := method(tile_x,
	self tile_x = tile_x;
	self;
);

RoomObj select := method(
	self is_current = 1;
	self;
);

RoomObj connect_tiles := method(
	i := 0;
	connection := call evalArgAt(i);

	while(connection != nil,
		plane_1 := connection at(0);
		coord_1 := connection at(1);
		coord_1 plane = plane_1;
		plane_2 := connection at(2);
		coord_2 := connection at(3);
		coord_2 plane = plane_2;

		new_connection := list(coord_1, coord_2);

		tile_connections append(new_connection);

		i = i + 1;
		connection = call evalArgAt(i)
	);

	self;
);

PlaneObj set_x_bounds := method(min_x, max_x,
	self min_x = min_x;
	self max_x = max_x;
	self;
);

PlaneObj set_y_bounds := method(min_y, max_y,
	self min_y = min_y;
	self max_y = max_y;
	self;
);

PlaneObj set_base_height := method(height,
	self h = height;
	self;
);

PlaneObj set_height_slope_x := method(hx,
	self hx = hx;
	self;
);

PlaneObj set_height_slope_y := method(hy,
	self hy = hy;
	self;
);

PlaneObj set_unwalkable := method(
	i := 0;
	coord := call evalArgAt(i);

	while(coord != nil,
		coord plane = self;
		if(coord != nil, unwalkable append(coord));
		i = i + 1;
		coord = call evalArgAt(i)
	);

	self;
);

EntityObj add_to_group := method(group,
	groups append(group);
);

EntityObj add_mapped := method(x, y, plane,
	mapped_c = MappedObj clone;
	mapped_c x := x;
	mapped_c y := y;
	mapped_c plane := plane;

	mapped_c;
);

EntityObj add_move := method(
	move_c = MoveObj clone;

	move_c;
);

EntityObj add_sprite := method(path,
	sprite_c = SpriteObj clone;
	sprite_c img = path;
	sprite_c scaling = 0;

	sprite_c;
);

EntityObj add_journey := method(room,
	journey_c = JourneyObj clone;
	journey_c room = room;

	self;
);

EntityObj add_animate := method(path,
	animate_c = AnimateObj clone;
	animate_c path = path;
	animate_c scaling = 0;
	animate_c images = List clone;
	animate_c current = nil;

	animate_c;
);

EntityObj add_click := method(
	click_c = ClickObj clone;
	click_c tile = nil;
	click_c x = 0;
	click_c y = 0;
	click_c direction = nil;
	
	click_c;
);

EntityObj add_interact := method(label,
	interact_c = InteractObj clone;
	interact_c label = label;

	interact_c;
);

SpriteObj enable_scaling := method(enabled,
    scaling = enabled;

	self;
);

AnimateObj enable_scaling := method(enabled,
    scaling = enabled;

	self;
);

AnimateObj load_sprite := method(key, img,
	images append(list(key, img));

	self;
);

AnimateObj use_sprite := method(current,
	self current = current;

	self;
);

ClickObj set_interact_point := method(tile, x, y, direction,
	self tile = tile;
	self x = x;
	self y = y;
	self direction = direction;

	self;
);
