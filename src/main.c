#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./objects/graphics.h"
#include "./ECS/ECS.h"
#include "./ECS/Components.h"
#include "./objects/game.h"
#include "./groups.h"

int main(int argc, char *argv[]) {
    // Sets up the game.
    GamePtr game = (GamePtr) malloc(sizeof(Game));
    Game_init(game, 1000, 700);

    Uint32 frame_start;
    int frame_time;
    const int fps = 60;
    const int frame_delay = 1000 / fps;

    // Main loop
    while (game->running) {
    	frame_start = SDL_GetTicks();

		Game_handle_events(game);
    	Game_update(game);
    	Game_render(game);

    	frame_time = SDL_GetTicks() - frame_start;

    	if (frame_delay > frame_time) {
    		SDL_Delay(frame_delay - frame_time);
    	}
    }

    Game_clean(game);

    return 0;
}
