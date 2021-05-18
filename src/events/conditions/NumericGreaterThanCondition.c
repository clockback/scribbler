#include <stdarg.h>

#include "NumericGreaterThanCondition.h"

void NumericGreaterThanCondition_init(void * me_void, va_list * args) {
	NumericGreaterThanConditionPtr me = (NumericGreaterThanConditionPtr)me_void;
	me->left = va_arg(*args, NumericPtr);
	me->right = va_arg(*args, NumericPtr);
}

bool NumericGreaterThanCondition_evaluate(void * me_void) {
	NumericGreaterThanConditionPtr me = (NumericGreaterThanConditionPtr)me_void;
	return Numeric_evaluate(me->left) > Numeric_evaluate(me->right);
}
