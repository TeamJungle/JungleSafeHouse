#include "chaser_object.hpp"
#include "game.hpp"
#include "assets.hpp"
#include "move.hpp"
#include "player_object.hpp"

#include <graphics.hpp>

chaser_object::chaser_object() {
	animation.fps = 15.0f;
	direction = 0;
}

void chaser_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	ne::game_object::update(world, chunk);
	world->each<player_object>([&](player_object* player) {
		if (player->direction != 0) {
			return;
		}
		auto player_move = player->component<game_object_move_component>();
		if (player_move->is_jumping()) {
			auto move = component<game_object_move_component>();
			move->jump();
		}
	});
	transform.position.x += 5.0f;
	direction = 1;
	component<game_object_move_component>()->is_running = true;
}

void chaser_object::draw() {
	auto move = component<game_object_move_component>();
	ne::texture* sprite = &textures.objects.player.idle[direction];
	if (move->is_running) {
		sprite = &textures.objects.chaser.run[direction];
	}
	if (move->is_jumping()) {
		sprite = &textures.objects.chaser.jump[direction];
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
