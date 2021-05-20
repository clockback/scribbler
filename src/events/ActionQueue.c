#include "ActionQueue.h"

void ActionQueue_init(
	ActionQueuePtr me, ActionPtr * actions, size_t no_actions
) {
	me->actions = (ActionPtr *) malloc(no_actions * sizeof(ActionPtr));
	me->no_actions = no_actions;
	for (size_t i = 0; i < no_actions; i ++) {
		me->actions[i] = (ActionPtr) malloc(no_actions * sizeof(Action));
		me->actions[i]->type = actions[i]->type;
		me->actions[i]->is_async = actions[i]->is_async;
		me->actions[i]->particulars = (void *) malloc(
			action_sizes[actions[i]->type]
		);
		memcpy(
			me->actions[i]->particulars, actions[i]->particulars,
			action_sizes[actions[i]->type]
		);
	}
}

bool ActionQueue_run(ActionQueuePtr me) {
	for (size_t i = 0; i < me->no_actions; i ++) {
		bool finished = Action_run(me->actions[i]);
		if (finished) {
			free(me->actions[i]->particulars);
			free(me->actions[i]);
			me->no_actions --;
			for (size_t j = i; j < me->no_actions; j ++) {
				me->actions[j] = me->actions[j + 1];
			}
			me->actions = (ActionPtr *) realloc(
				me->actions, me->no_actions * sizeof(ActionPtr)
			);
			i --;
			continue;
		}

		if (!me->actions[i]->is_async) {
			break;
		}
	}

	return me->no_actions == 0;
}
