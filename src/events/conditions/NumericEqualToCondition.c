#include <stdarg.h>

#include "NumericEqualToCondition.h"

void NumericEqualToCondition_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	NumericEqualToConditionPtr me = (NumericEqualToConditionPtr)me_void;
	IoSymbol * get_left = IoState_symbolWithCString_(
		io_handler->iostate, "left"
	);
	IoSymbol * get_right = IoState_symbolWithCString_(
		io_handler->iostate, "right"
	);
	IoObject * io_left = IoObject_getSlot_(io_particulars, get_left);
	IoObject * io_right = IoObject_getSlot_(io_particulars, get_right);

	me->left = (NumericPtr)malloc(sizeof(Numeric));
	me->right = (NumericPtr)malloc(sizeof(Numeric));

	IoHandler_process(io_handler, io_left, me->left);
	IoHandler_process(io_handler, io_right, me->right);
}

bool NumericEqualToCondition_evaluate(void * me_void) {
	NumericGreaterThanConditionPtr me = (NumericGreaterThanConditionPtr)me_void;
	return Numeric_evaluate(me->left) == Numeric_evaluate(me->right);
}
