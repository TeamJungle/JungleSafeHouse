#include "spikes_object.hpp"
#include "game.hpp"
#include "assets.hpp"

spikes_object::spikes_object() {
	
}

void spikes_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	
}

void spikes_object::draw() {
	ne::shader::set_transform(&transform);
	textures.objects.spikes[subtype].bind();
	still_quad().bind();
	still_quad().draw();
}

void spikes_object::write(ne::memory_buffer* buffer) {
	ne::game_object::write(buffer);
}

void spikes_object::read(ne::memory_buffer* buffer) {
	ne::game_object::read(buffer);
}

