#include "game_object.hpp"

void game_object::write(ne::memory_buffer* buffer) {
	buffer->write_transform3f(transform);
}

void game_object::read(ne::memory_buffer* buffer) {
	transform = buffer->read_transform3f();
}

void game_object::apply_gravity(game_world* world) {
	// TODO: Improve.
	int limit = 0;
	while (limit++ < 5 && transform.position.y + transform.scale.y < 0) {
		transform.position.y += 1.0f;
	}
}
