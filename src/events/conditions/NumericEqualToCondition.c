#include <stdarg.h>

#include "NumericEqualToCondition.h"

void NumericEqualToCondition_init(void * me_void, va_list * args) {
	NumericEqualToConditionPtr me = (NumericEqualToConditionPtr)me_void;
	me->left = va_arg(*args, NumericPtr);
	me->right = va_arg(*args, NumericPtr);
}

bool NumericEqualToCondition_evaluate(void * me_void) {
	NumericGreaterThanConditionPtr me = (NumericGreaterThanConditionPtr)me_void;
	return Numeric_evaluate(me->left) == Numeric_evaluate(me->right);
}
