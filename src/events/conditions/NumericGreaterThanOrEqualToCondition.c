#include <stdarg.h>

#include "NumericGreaterThanOrEqualToCondition.h"

void NumericGreaterThanOrEqualToCondition_init(void * me_void, va_list * args) {
	NumericGreaterThanOrEqualToConditionPtr me = (
		(NumericGreaterThanOrEqualToConditionPtr)me_void
	);
	me->left = va_arg(*args, NumericPtr);
	me->right = va_arg(*args, NumericPtr);
}

bool NumericGreaterThanOrEqualToCondition_evaluate(void * me_void) {
	NumericGreaterThanOrEqualToConditionPtr me = (
		(NumericGreaterThanOrEqualToConditionPtr)me_void
	);
	return Numeric_evaluate(me->left) >= Numeric_evaluate(me->right);
}
