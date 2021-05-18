#include <stdarg.h>

#include "NumericLessThanOrEqualToCondition.h"

void NumericLessThanOrEqualToCondition_init(void * me_void, va_list * args) {
	NumericLessThanOrEqualToConditionPtr me = (
		(NumericLessThanOrEqualToConditionPtr)me_void
	);
	me->left = va_arg(*args, NumericPtr);
	me->right = va_arg(*args, NumericPtr);
}

bool NumericLessThanOrEqualToCondition_evaluate(void * me_void) {
	NumericLessThanOrEqualToConditionPtr me = (
		(NumericLessThanOrEqualToConditionPtr)me_void
	);
	return Numeric_evaluate(me->left) <= Numeric_evaluate(me->right);
}
