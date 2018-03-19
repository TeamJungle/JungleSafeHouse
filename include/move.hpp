#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>

class game_state;
class game_world;

class game_object_move_component : public ne::game_object_component {
public:

	bool is_sliding = false;
	bool is_running = false;
	int max_double_jumps = 1;
	float max_jump_force = 10.0f;
	float max_speed = 5.0f;

	game_object_move_component(ne::game_object* parent) : ne::game_object_component(parent) {}

	void update(ne::game_world* world, ne::game_world_chunk* chunk) override;
	void jump();
	void slide(int direction);
	bool move(ne::game_world* world, bool left, bool right);

	bool is_jumping() const;

	void stop();

private:

	void apply_gravity(ne::game_world* world);

	// How many overlapping jumps are ongoing.
	int current_jumps = 0;

	float jump_force = 0.0f;

	float acceleration = 0.2f;
	float speed = 0.0f;

	float slide_left = 0.0f;

};
