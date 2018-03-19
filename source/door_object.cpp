#include "door_object.hpp"
#include "game.hpp"
#include "assets.hpp"
#include "player_object.hpp"

#include <game_world.hpp>

void door_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	is_near_player = world->first<player_object>()->collision_transform().collides_with(transform);
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
	level_label.font = &fonts.hud;
	level_label.render(std::to_string(leads_to_level_num));
	level_label.transform.position.x = transform.position.x + transform.scale.width / 2.0f - level_label.transform.scale.width / 2.0f;
	level_label.transform.position.y = transform.position.y + transform.scale.height / 2.0f - level_label.transform.scale.height / 6.0f;
	ne::shader::set_color(1.0f, 0.4f, 0.1f, 0.7f);
	level_label.transform.rotation.z = transform.rotation.z;
	level_label.draw();
	if (opening_cost > 0 && is_near_player) {
		cost_label.font = &fonts.hud_small;
		cost_label.render(std::to_string(opening_cost));
		cost_label.transform.position.x = transform.position.x + transform.scale.width / 2.0f - cost_label.transform.scale.width / 2.0f;
		cost_label.transform.position.y = transform.position.y + 8.0f;
		cost_label.transform.rotation.z = transform.rotation.z;
		ne::shader::set_color(0.9f, 0.8f, 0.1f, 1.0f);
		cost_label.draw();
		ne::shader::set_color(1.0f);
		ne::transform3f coin_transform = cost_label.transform;
		coin_transform.scale.xy = textures.objects.item[ITEM_COIN].frame_size().to<float>();
		coin_transform.position.x -= coin_transform.scale.width + 4.0f;
		textures.objects.item[ITEM_COIN].bind();
		ne::shader::set_transform(&coin_transform);
		animated_quad().bind();
		coin_animation.draw();
	}
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
