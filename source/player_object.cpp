#include "player_object.hpp"
#include "game.hpp"
#include "assets.hpp"

#include <graphics.hpp>

player_object::player_object(game_state* game) : game_object(game) {
	up_hit = game->input.up_hit.listen([]() {
		// jump
	});
	animation.fps = 30.0f;
}

player_object::~player_object() {
	game->input.up_hit.erase(&up_hit);
}

void player_object::update(game_world* world) {
	game_object::apply_gravity(world);
	is_running = false;
	bool up = game->input.up.is_active();
	bool left = game->input.left.is_active();
	bool down = game->input.down.is_active();
	bool right = game->input.right.is_active();
	if (right) {
		transform.position.x += 5.0f;
		direction = 1;
		is_running = true;
	}
	if (left) {
		transform.position.x -= 5.0f;
		direction = 0;
		is_running = true;
	}
}

void player_object::draw() {
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

void player_object::write(ne::memory_buffer* buffer) {

}

void player_object::read(ne::memory_buffer* buffer) {

}
