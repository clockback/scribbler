#include "Numeric.h"


size_t numeric_sizes[MAX_NUMERICS];

void (*init_for_numeric_functions[MAX_NUMERICS]) (
	void * me_void, va_list * args
);

double (*evaulate_for_numeric_functions[MAX_NUMERICS]) (void * me_void);

void Numeric_init(NumericPtr me, NumericType type, va_list * args) {
	me->type = type;
	me->particulars = (void *) malloc(numeric_sizes[type]);
	init_for_numeric_functions[type](me->particulars, args);
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
