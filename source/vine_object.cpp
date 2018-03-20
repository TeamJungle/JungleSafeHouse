#include "vine_object.hpp"
#include "assets.hpp"
#include "game.hpp"

#include <game_world.hpp>

void vine_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	if (health < 1.0f) {
		health -= 0.1f;
		if (health < 0.0f) {
			health = 0.0f;
		}
	}
}

void vine_object::draw() {
	ne::transform3f draw_transform = transform;
	if (flip_x) {
		draw_transform.position.x += draw_transform.scale.width;
		draw_transform.scale.width = -draw_transform.scale.width;
	}
	ne::shader::set_transform(&draw_transform);
	textures.objects.vine[subtype].bind();
	still_quad().bind();
	if (health < 1.0f) {
		ne::shader::set_color({ 1.0f, 1.0f, 1.0f, health });
		still_quad().draw();
		ne::shader::set_color(1.0f);
	} else {
		still_quad().draw();
	}
}

void vine_object::write(ne::memory_buffer* buffer) {
	ne::game_object::write(buffer);
	buffer->write_uint8(flip_x ? 1 : 0);
}

void vine_object::read(ne::memory_buffer* buffer) {
	ne::game_object::read(buffer);
	flip_x = (buffer->read_uint8() != 0);
}
