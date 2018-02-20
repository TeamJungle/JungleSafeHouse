#include "move.hpp"

void game_object_move_component::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	apply_gravity(world);
	if (is_sliding) {
		if (slide_left > 0.0f) {
			object()->transform.position.x += 6.0f;
			slide_left -= 6.0f;
			if (slide_left < 1.0f) {
				slide_left = 0.0f;
				is_sliding = false;
			}
		} else {
			object()->transform.position.x -= 6.0f;
			slide_left += 6.0f;
			if (slide_left > -1.0f) {
				slide_left = 0.0f;
				is_sliding = false;
			}
		}
	}
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
	if (is_sliding) {
		return;
	}
	// NOTE: Make this check more flexible in the future to allow for multijumps.
	if (current_jumps > 1) {
		return;
	}
	current_jumps++;
	jump_force = max_jump_force;
}

void game_object_move_component::slide(int direction) {
	if (is_sliding) {
		return;
	}
	if (current_jumps > 0) {
		return;
	}
	is_sliding = true;
	slide_left = 400.0f * (direction == 1 ? 1.0f : -1.0f);
}

void game_object_move_component::move(bool left, bool right) {
	if (is_sliding) {
		return;
	}
	is_running = false;
	if (right && left) {
		return;
	}
	if (!right && !left) {
		if (speed > 0.0f) {
			speed -= acceleration * 5.0f;
			if (parent->direction == 1) {
				right = true;
			} else if (parent->direction == 0) {
				left = true;
			}
		} else {
			return;
		}
	}
	is_running = true;
	if (speed < 5.0f) {
		speed += acceleration;
	}
	if (right) {
		parent->transform.position.x += speed;
		parent->direction = 1;
	}
	if (left) {
		parent->transform.position.x -= speed;
		parent->direction = 0;
	}
}

bool game_object_move_component::is_jumping() const {
	return current_jumps > 0;
}

void game_object_move_component::stop() {
	is_running = false;
	speed = -1.0f;
}
