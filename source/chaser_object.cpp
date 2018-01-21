#include "chaser_object.hpp"
#include "game.hpp"
#include "assets.hpp"

#include <graphics.hpp>

chaser_object::chaser_object(game_state* game) : game_object(game) {
	animation.fps = 25.0f;
}

chaser_object::~chaser_object() {
	
}

void chaser_object::update(game_world* world) {
	game_object::apply_gravity(world);
	transform.position.x += 4.0f;
	direction = 1;
	is_running = true;
}

void chaser_object::draw() {
	ne::texture* sprite = &textures.objects.player.idle[direction];
	if (is_running) {
		sprite = &textures.objects.player.run[direction];
	}
	transform.scale.xy = {
		(float)(sprite->size.x / sprite->parameters.frames),
		(float)sprite->size.y
	};
	ne::set_drawing_shape(1);
	ne::shader::set_transform(&transform);
	sprite->bind();
	animation.draw(true);
}

void chaser_object::write(ne::memory_buffer* buffer) {

}

void chaser_object::read(ne::memory_buffer* buffer) {

}
