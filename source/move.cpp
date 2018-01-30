#include "move.hpp"

void game_object_move_component::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	apply_gravity(world);
}

void game_object_move_component::apply_gravity(ne::game_world* world) {
	if (jump_force > 0.0f) {
		parent->transform.position.y -= jump_force;
		jump_force -= 0.2f;
		if (jump_force < 0.0f) {
			jump_force = 0.0f;
		}
	}
	int limit = 0;
	while (limit < 5 && parent->transform.position.y + parent->transform.scale.y < 600.0f) {
		parent->transform.position.y += 1.0f;
		limit++;
	}
	if (limit == 0) {
		current_jumps = 0;
	}
}

void game_object_move_component::jump() {
	// NOTE: Make this check more flexible in the future to allow for multijumps.
	if (current_jumps > 1) {
		return;
	}
	current_jumps++;
	jump_force = max_jump_force;
}
