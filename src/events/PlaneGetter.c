#include "PlaneGetter.h"


size_t plane_getter_sizes[MAX_PLANE_GETTERS];

void (*init_for_plane_getter_functions[MAX_PLANE_GETTERS]) (
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);

PlanePtr (*evaulate_for_plane_getter_functions[MAX_PLANE_GETTERS]) (
	void * me_void
);

void PlaneGetter_init(
	PlaneGetterPtr me, IoObject * base, IoHandler * io_handler, GamePtr game
) {
	const char * plane_getter_type_name = IoObject_name(base);

	if (strcmp(plane_getter_type_name, "PlaneObj") == 0) {
		me->type = RAW_PLANE;
	}
	else {
		printf("Unknown plane getter type: %s\n", plane_getter_type_name);
		IoObject_print(base);
		printf("\n");
		exit(-1);
	}

	me->particulars = (void *) malloc(plane_getter_sizes[me->type]);
	init_for_plane_getter_functions[me->type](
		me->particulars, base, io_handler, game
	);
}

PlanePtr PlaneGetter_evaluate(PlaneGetterPtr me) {
	return evaulate_for_plane_getter_functions[me->type](me->particulars);
}

void pre_init_plane_getter_functions() {
	init_for_plane_getter_functions[RAW_PLANE] = RawPlanePlaneGetter_init;
}

void pre_init_plane_getter_sizes() {
	plane_getter_sizes[RAW_PLANE] = sizeof(RawPlanePlaneGetter);
}

void pre_init_plane_getter_evaluate() {
	evaulate_for_plane_getter_functions[RAW_PLANE] = (
		RawPlanePlaneGetter_evaluate
	);
}

void init_plane_getters() {
	pre_init_plane_getter_functions();
	pre_init_plane_getter_sizes();
	pre_init_plane_getter_evaluate();
}
