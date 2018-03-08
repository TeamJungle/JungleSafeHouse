#include "door_object.hpp"
#include "game.hpp"
#include "assets.hpp"

#include <game_world.hpp>

void door_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	
}

void door_object::draw() {
	ne::shader::set_transform(&transform);
	if (is_open) {
		textures.objects.door.open.bind();
	} else {
		textures.objects.door.door[subtype].bind();
	}
	still_quad().bind();
	still_quad().draw();
	if (leads_to_level_num < 1 || is_open) {
		return;
	}
	label.font = &fonts.hud;
	label.render(std::to_string(leads_to_level_num));
	label.transform.position.x = transform.position.x + transform.scale.width / 2.0f - label.transform.scale.width / 2.0f;
	label.transform.position.y = transform.position.y + transform.scale.height / 2.0f - label.transform.scale.height / 6.0f;
	ne::shader::set_color(1.0f, 0.4f, 0.1f, 0.7f);
	label.draw();
	ne::shader::set_color(1.0f);
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
