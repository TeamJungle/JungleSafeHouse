#include <GLEW/glew.h>
#include <SDL/SDL.h>

#include "game.hpp"
#include "assets.hpp"
#include "move.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"
#include "decoration_object.hpp"
#include "menu.hpp"

#include <engine.hpp>
#include <window.hpp>
#include <graphics.hpp>

void start() {
	// Black background color.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Start the game in maximised window mode.
	ne::maximise_window();

	// Load all the assets. (blocks)
	load_assets();

	// Create shapes.
	still_quad().create();
	still_quad().make_quad();
	animated_quad().create();
	animated_quad().make_quad();

	// Setup chunk sizes.
	ne::game_world_tile::pixel_size = 16;
	ne::game_world_chunk::tiles_per_row = 128;
	ne::game_world_chunk::tiles_per_column = 32;

	// Configure objects.
	ne::game_object_factory::add_default_component([](ne::game_object* object) {
		return new game_object_move_component(object);
	});

	ne::game_object_factory::define(OBJECT_TYPE_PLAYER, [] {
		return new player_object();
	}, [](int subtype) {
		return &textures.objects.player.idle[0];
	}, [](int subtype) -> std::vector<ne::texture*> {
		return { nullptr };
	});

	ne::game_object_factory::define(OBJECT_TYPE_CHASER, [] {
		return new chaser_object();
	}, [](int subtype) {
		return &textures.objects.chaser.idle[0];
	}, [](int subtype) -> std::vector<ne::texture*> {
		return { nullptr };
	});

	ne::game_object_factory::define(OBJECT_TYPE_DECORATION, [] {
		return new decoration_object();
	}, [](int subtype) {
		return &textures.objects.decoration[subtype];
	}, [](int subtype) -> std::vector<ne::texture*> {
		return { nullptr };
	});

	// Turn off VSync.
	ne::set_swap_interval(ne::swap_interval::immediate);

	// Start the game.
	ne::swap_state<menu_state>();
}

void stop() {
	destroy_assets();
}

int main(int argc, char** argv) {
	ne::start_engine("Jungle Safe House", 800, 600);
	return ne::enter_loop(start, stop);
}
