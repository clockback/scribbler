#include "EntityGetter.h"


size_t entity_getter_sizes[MAX_ENTITY_GETTERS];

void (*init_for_entity_getter_functions[MAX_ENTITY_GETTERS]) (
	void * me_void, va_list * args
);

EntityPtr (*evaulate_for_entity_getter_functions[MAX_ENTITY_GETTERS]) (
	void * me_void
);

void EntityGetter_init(
	EntityGetterPtr me, EntityGetterType type, va_list * args
) {
	me->type = type;
	me->particulars = (void *) malloc(entity_getter_sizes[type]);
	init_for_entity_getter_functions[type](me->particulars, args);
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
