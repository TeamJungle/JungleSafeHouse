#include "player_object.hpp"
#include "game.hpp"
#include "assets.hpp"
#include "move.hpp"

#include <graphics.hpp>

player_object::player_object() {
	up_hit = input().up_hit.listen([this]() {
		component<game_object_move_component>()->jump();
	});
	animation.fps = 30.0f;
}

player_object::~player_object() {
	input().up_hit.erase(&up_hit);
}

void player_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	ne::game_object::update(world, chunk);
	auto move = component<game_object_move_component>();
	bool up = input().up.is_active();
	bool left = input().left.is_active();
	bool down = input().down.is_active();
	bool right = input().right.is_active();
	move->move(left, right);
}

void player_object::draw() {
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

void player_object::write(ne::memory_buffer* buffer) {

}

void player_object::read(ne::memory_buffer* buffer) {

}
