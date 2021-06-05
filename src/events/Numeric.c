#include "Numeric.h"


size_t numeric_sizes[MAX_NUMERICS];

void (*init_for_numeric_functions[MAX_NUMERICS]) (
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);

double (*evaulate_for_numeric_functions[MAX_NUMERICS]) (void * me_void);

void Numeric_init(
	NumericPtr me, IoObject * base, IoHandler * io_handler, GamePtr game
) {
	const char * numeric_type_name = IoObject_name(base);

	if (ISNUMBER(base)) {
		me->type = RAW_NUMERIC;
	}
	else if (strcmp(numeric_type_name, "EntityGetXNumericObj") == 0) {
		me->type = ENTITY_GET_X;
	}
	else if (strcmp(numeric_type_name, "EntityGetYNumericObj") == 0) {
		me->type = ENTITY_GET_Y;
	}
	else {
		printf("Unknown numeric type: %s\n", numeric_type_name);
		IoObject_print(base);
		printf("\n");
		exit(-1);
	}

	me->particulars = (void *) malloc(numeric_sizes[me->type]);
	init_for_numeric_functions[me->type](
		me->particulars, base, io_handler, game
	);
}

double Numeric_evaluate(NumericPtr me) {
	return evaulate_for_numeric_functions[me->type](me->particulars);
}

void pre_init_numeric_functions() {
	init_for_numeric_functions[RAW_NUMERIC] = RawNumeric_init;
	init_for_numeric_functions[ENTITY_GET_X] = EntityGetX_init;
	init_for_numeric_functions[ENTITY_GET_Y] = EntityGetY_init;
}

void pre_init_numeric_sizes() {
	numeric_sizes[RAW_NUMERIC] = sizeof(EntityGetX);
	numeric_sizes[ENTITY_GET_X] = sizeof(EntityGetX);
	numeric_sizes[ENTITY_GET_Y] = sizeof(EntityGetY);
}

void pre_init_numeric_evaluate() {
	evaulate_for_numeric_functions[RAW_NUMERIC] = RawNumeric_evaluate;
	evaulate_for_numeric_functions[ENTITY_GET_X] = EntityGetX_evaluate;
	evaulate_for_numeric_functions[ENTITY_GET_Y] = EntityGetY_evaluate;
}

void init_numerics() {
	pre_init_numeric_functions();
	pre_init_numeric_sizes();
	pre_init_numeric_evaluate();
}
