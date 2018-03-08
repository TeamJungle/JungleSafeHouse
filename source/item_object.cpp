#include "item_object.hpp"
#include "assets.hpp"
#include "game.hpp"
#include "player_object.hpp"

#include <game_world.hpp>

void item_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	
}

void item_object::draw() {
	ne::shader::set_transform(&transform);
	textures.objects.item[subtype].bind();
	animated_quad().bind();
	animation.fps = 9.0f;
	animation.draw(true);
}

void item_object::write(ne::memory_buffer* buffer) {
	ne::game_object::write(buffer);
}

void item_object::read(ne::memory_buffer* buffer) {
	ne::game_object::read(buffer);
}
