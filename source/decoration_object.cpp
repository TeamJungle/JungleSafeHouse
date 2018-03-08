#include "decoration_object.hpp"
#include "assets.hpp"
#include "game.hpp"

#include <game_world.hpp>

void decoration_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	
}

void decoration_object::draw() {
	ne::transform3f draw_transform = transform;
	if (flip_x) {
		draw_transform.position.x += draw_transform.scale.width;
		draw_transform.scale.width = -draw_transform.scale.width;
	}
	if (flip_y) {
		draw_transform.position.y += draw_transform.scale.height;
		draw_transform.scale.height = -draw_transform.scale.height;
	}
	ne::shader::set_transform(&draw_transform);
	textures.objects.decoration[subtype].bind();
	still_quad().bind();
	still_quad().draw();
}

void decoration_object::write(ne::memory_buffer* buffer) {
	ne::game_object::write(buffer);
	buffer->write_uint8((flip_x ? 1 : 0));
	buffer->write_uint8((flip_y ? 1 : 0));
}

void decoration_object::read(ne::memory_buffer* buffer) {
	ne::game_object::read(buffer);
	flip_x = (buffer->read_uint8() != 0);
	flip_y = (buffer->read_uint8() != 0);
}
