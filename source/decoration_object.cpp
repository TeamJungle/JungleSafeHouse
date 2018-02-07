#include "decoration_object.hpp"
#include "assets.hpp"
#include "game.hpp"

#include <game_world.hpp>

void decoration_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	transform.scale.xy = textures.objects.decoration[subtype].size.to<float>() / 5.0f;
}

void decoration_object::draw() {
	ne::shader::set_color(1.0f);
	ne::shader::set_transform(&transform);
	textures.objects.decoration[subtype].bind();
	still_quad().bind();
	still_quad().draw();
}

void decoration_object::write(ne::memory_buffer* buffer) {

}

void decoration_object::read(ne::memory_buffer* buffer) {

}
