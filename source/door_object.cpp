#include "door_object.hpp"
#include "game.hpp"
#include "assets.hpp"

#include <game_world.hpp>

void door_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	
}

void door_object::draw() {
	ne::shader::set_color(1.0f);
	ne::shader::set_transform(&transform);
	if (is_open) {
		textures.objects.door.open.bind();
	} else {
		textures.objects.door.door[subtype].bind();
	}
	animated_quad().bind();
	animation.fps = 9.0f;
	animation.draw(true);
}

void door_object::write(ne::memory_buffer* buffer) {
	ne::game_object::write(buffer);
	buffer->write_uint8((is_open ? 1 : 0));
	buffer->write_int32(leads_to_level_num);
	buffer->write_int32(opening_cost);
}

void door_object::read(ne::memory_buffer* buffer) {
	ne::game_object::read(buffer);
	is_open = (buffer->read_uint8() != 0);
	leads_to_level_num = buffer->read_int32();
	opening_cost = buffer->read_int32();
}
