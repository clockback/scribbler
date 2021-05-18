#include <stdarg.h>

#include "NumericLessThanCondition.h"

void NumericLessThanCondition_init(void * me_void, va_list * args) {
	NumericLessThanConditionPtr me = (NumericLessThanConditionPtr)me_void;
	me->left = va_arg(*args, NumericPtr);
	me->right = va_arg(*args, NumericPtr);
}

bool NumericLessThanCondition_evaluate(void * me_void) {
	NumericLessThanConditionPtr me = (NumericLessThanConditionPtr)me_void;
	return Numeric_evaluate(me->left) < Numeric_evaluate(me->right);
}
