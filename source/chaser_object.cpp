#include "chaser_object.hpp"
#include "game.hpp"
#include "assets.hpp"
#include "move.hpp"

#include <graphics.hpp>

chaser_object::chaser_object() {
	animation.fps = 25.0f;
}

void chaser_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	ne::game_object::update(world, chunk);
	transform.position.x += 4.0f;
	direction = 1;
	component<game_object_move_component>()->is_running = true;
}

void chaser_object::draw() {
	ne::texture* sprite = &textures.objects.player.idle[direction];
	if (component<game_object_move_component>()->is_running) {
		sprite = &textures.objects.player.run[direction];
	}
	transform.scale.xy = {
		(float)(sprite->size.x / sprite->parameters.frames),
		(float)sprite->size.y
	};
	animated_quad().bind();
	ne::shader::set_transform(&transform);
	sprite->bind();
	animation.draw(true);
}

void chaser_object::write(ne::memory_buffer* buffer) {

}

void chaser_object::read(ne::memory_buffer* buffer) {

}
