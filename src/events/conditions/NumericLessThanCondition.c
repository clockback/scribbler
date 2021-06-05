#include <stdarg.h>

#include "NumericLessThanCondition.h"

void NumericLessThanCondition_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	NumericLessThanConditionPtr me = (NumericLessThanConditionPtr)me_void;
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

bool NumericLessThanCondition_evaluate(void * me_void) {
	NumericLessThanConditionPtr me = (NumericLessThanConditionPtr)me_void;
	return Numeric_evaluate(me->left) < Numeric_evaluate(me->right);
}
