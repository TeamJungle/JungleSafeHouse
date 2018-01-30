#include "terrain_object.hpp"
#include "assets.hpp"
#include "game.hpp"

#include <game_world.hpp>

void terrain_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	ne::game_object::update(world, chunk);
	transform.scale.xy = ne::game_world_chunk::pixel_size().to<float>();
}

void terrain_object::draw() {
	ne::shader::set_transform(&transform);
	ne::shader::set_color(0.2f, 0.8f, 0.3f, 1.0f);
	textures.blank.bind();
	still_quad().bind();
	still_quad().draw();
	ne::shader::set_color(1.0f);
}

void terrain_object::write(ne::memory_buffer* buffer) {

}

void terrain_object::read(ne::memory_buffer* buffer) {

}
