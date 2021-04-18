#include "./globals.h"

void Globals_init(
	GlobalsPtr me, ScreenPtr screen, RoomPtr room, FontPtr font
) {
	me->screen = screen;
	me->room = room;
	me->font = font;
}
