#include "platform_object.hpp"
#include "assets.hpp"
#include "game.hpp"

#include <game_world.hpp>

void platform_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	
}

void platform_object::draw() {
	// Makes it easier to handle this object in the editor.
	transform.scale.xy = collision.size;
}

void platform_object::write(ne::memory_buffer* buffer) {
	ne::game_object::write(buffer);
}

void platform_object::read(ne::memory_buffer* buffer) {
	ne::game_object::read(buffer);
}
