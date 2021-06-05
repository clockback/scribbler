#include "EntityGetter.h"


size_t entity_getter_sizes[MAX_ENTITY_GETTERS];

void (*init_for_entity_getter_functions[MAX_ENTITY_GETTERS]) (
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);

EntityPtr (*evaulate_for_entity_getter_functions[MAX_ENTITY_GETTERS]) (
	void * me_void
);

void EntityGetter_init(
	EntityGetterPtr me, IoObject * base, IoHandler * io_handler, GamePtr game
) {
	const char * entity_getter_type_name = IoObject_name(base);

	if (strcmp(entity_getter_type_name, "EntityObj") == 0) {
		me->type = RAW_ENTITY;
	}
	else if (
		strcmp(entity_getter_type_name, "EntityWithNameEntityGetterObj") == 0
	) {
		me->type = ENTITY_WITH_NAME;
	}
	else {
		printf("Unknown entity getter type: %s\n", entity_getter_type_name);
		IoObject_print(base);
		printf("\n");
		exit(-1);
	}

	me->particulars = (void *) malloc(entity_getter_sizes[me->type]);
	init_for_entity_getter_functions[me->type](
		me->particulars, base, io_handler, game
	);
}

EntityPtr EntityGetter_evaluate(EntityGetterPtr me) {
	return evaulate_for_entity_getter_functions[me->type](me->particulars);
}

void pre_init_entity_getter_functions() {
	init_for_entity_getter_functions[RAW_ENTITY] = RawEntityEntityGetter_init;
	init_for_entity_getter_functions[ENTITY_WITH_NAME] = (
		EntityWithNameEntityGetter_init
	);
}

void pre_init_entity_getter_sizes() {
	entity_getter_sizes[RAW_ENTITY] = sizeof(RawEntityEntityGetter);
	entity_getter_sizes[ENTITY_WITH_NAME] = sizeof(EntityWithNameEntityGetter);
}

void pre_init_entity_getter_evaluate() {
	evaulate_for_entity_getter_functions[RAW_ENTITY] = (
		RawEntityEntityGetter_evaluate
	);
	evaulate_for_entity_getter_functions[ENTITY_WITH_NAME] = (
		EntityWithNameEntityGetter_evaluate
	);
}

void init_entity_getters() {
	pre_init_entity_getter_functions();
	pre_init_entity_getter_sizes();
	pre_init_entity_getter_evaluate();
}
