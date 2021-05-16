#include "./globals.h"

void Globals_init(
	GlobalsPtr me, ScreenPtr screen, RoomPtr room, FontPtr font,
	ScenarioManagerPtr scenario_manager
) {
	me->screen = screen;
	me->room = room;
	me->font = font;
	me->scenario_manager = scenario_manager;
}
