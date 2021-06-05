#include "Condition.h"


size_t condition_sizes[MAX_CONDITIONS];
void (*init_for_condition_functions[MAX_CONDITIONS]) (
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);
bool (*evaluate_for_condition_functions[MAX_CONDITIONS]) (void * me_void);


void pre_init_condition_functions() {
	init_for_condition_functions[NUMERIC_LESS_THAN_OR_EQUAL_TO] = (
		NumericLessThanOrEqualToCondition_init
	);
	init_for_condition_functions[NUMERIC_LESS_THAN] = (
		NumericLessThanCondition_init
	);
	init_for_condition_functions[NUMERIC_GREATER_THAN_OR_EQUAL_TO] = (
		NumericGreaterThanOrEqualToCondition_init
	);
	init_for_condition_functions[NUMERIC_GREATER_THAN] = (
		NumericGreaterThanCondition_init
	);
	init_for_condition_functions[NUMERIC_EQUAL_TO] = (
		NumericEqualToCondition_init
	);
}

void pre_init_condition_sizes() {
	condition_sizes[NUMERIC_LESS_THAN_OR_EQUAL_TO] = sizeof(
		NumericLessThanOrEqualToCondition
	);
	condition_sizes[NUMERIC_LESS_THAN] = sizeof(NumericLessThanCondition);
	condition_sizes[NUMERIC_GREATER_THAN_OR_EQUAL_TO] = sizeof(
		NumericGreaterThanOrEqualToCondition
	);
	condition_sizes[NUMERIC_GREATER_THAN] = sizeof(NumericGreaterThanCondition);
	condition_sizes[NUMERIC_EQUAL_TO] = sizeof(NumericEqualToCondition);
}

void pre_init_condition_evaluate() {
	evaluate_for_condition_functions[NUMERIC_LESS_THAN_OR_EQUAL_TO] = (
		NumericLessThanOrEqualToCondition_evaluate
	);
	evaluate_for_condition_functions[NUMERIC_LESS_THAN] = (
		NumericLessThanCondition_evaluate
	);
	evaluate_for_condition_functions[NUMERIC_GREATER_THAN_OR_EQUAL_TO] = (
		NumericGreaterThanOrEqualToCondition_evaluate
	);
	evaluate_for_condition_functions[NUMERIC_GREATER_THAN] = (
		NumericGreaterThanCondition_evaluate
	);
	evaluate_for_condition_functions[NUMERIC_EQUAL_TO] = (
		NumericEqualToCondition_evaluate
	);
}

void init_conditions() {
	pre_init_condition_functions();
	pre_init_condition_sizes();
	pre_init_condition_evaluate();
}
