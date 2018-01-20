#include "game_object.hpp"

void game_object::write(ne::memory_buffer* buffer) {
	buffer->write_transform3f(transform);
}

void game_object::read(ne::memory_buffer* buffer) {
	transform = buffer->read_transform3f();
}

void game_object::apply_gravity(game_world* world) {
	if (jump_force > 0.0f) {
		transform.position.y -= jump_force;
		jump_force -= 0.2f;
		if (jump_force < 0.0f) {
			jump_force = 0.0f;
		}
	}
	int limit = 0;
	while (limit < 5 && transform.position.y + transform.scale.y < 0) {
		transform.position.y += 1.0f;
		limit++;
	}
	if (limit == 0) {
		current_jumps = 0;
	}
}

void game_object::jump() {
	// NOTE: Make this check more flexible in the future to allow for multijumps.
	if (current_jumps > 0) {
		return;
	}
	current_jumps++;
	jump_force = max_jump_force;
}
