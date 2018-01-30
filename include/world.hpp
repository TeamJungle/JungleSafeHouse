#pragma once

#include <game_world.hpp>
#include <game_object.hpp>

class player_object;
class game_state;

class game_world : public ne::game_world {
public:

	player_object* player = nullptr;

	game_world();

	void update() override;
	void draw(const ne::transform3f& view) override;
	
};
