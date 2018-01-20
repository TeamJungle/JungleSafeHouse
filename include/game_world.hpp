#pragma once

#include "game_object_container.hpp"

class player_object;
class game_state;

class game_world_terrain {
public:

	ne::transform3f area;

	void draw();

};

class game_world {
public:

	player_object* player = nullptr;
	game_object_container objects;
	game_world_terrain terrain;

	game_world(game_state* game);

	void update();
	void draw(const ne::transform3f& view);

private:

	game_state* game = nullptr;

};
