#include <GLEW/glew.h>
#include <SDL/SDL.h>

#include "game.hpp"
#include "assets.hpp"

#include <engine.hpp>
#include <window.hpp>
#include <graphics.hpp>

void start() {
	// Start the game in maximised window mode.
	ne::maximise_window();

	// Set up some shapes for reuse.
	ne::create_quad();
	ne::create_quad();

	// Load all the assets. (blocks)
	load_assets();

	// Turn off VSync.
	ne::set_swap_interval(NE_SWAP_INTERVAL_IMMEDIATE);

	// Start the game.
	ne::swap_state<game_state>();
}

void stop() {
	destroy_assets();
}

int main(int argc, char** argv) {
	ne::start_engine("Jungle Safe House", 800, 600);
	return ne::enter_loop(start, stop);
}
