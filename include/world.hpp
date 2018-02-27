#pragma once

#include "game_world_background.hpp"
#include "game_object_types.hpp"

#include <game_world.hpp>
#include <game_object.hpp>
#include <timer.hpp>

class player_object;
class game_state;

class game_world : public ne::game_world {
public:

	struct {
		// Background:
		game_world_background background;
		game_world_background trees;
		game_world_background fog_back;
		game_world_background mid;
		// Foreground:
		game_world_background top;
		game_world_background top_lines;
		game_world_background bottom;
		game_world_background fog_front;
	} backgrounds;

	game_world();

	void generate();

	void update() override;
	void draw(const ne::transform3f& view) override;
	
};
