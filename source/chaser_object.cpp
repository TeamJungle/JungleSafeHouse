#include "chaser_object.hpp"
#include "game.hpp"
#include "assets.hpp"
#include "move.hpp"
#include "player_object.hpp"
#include "decoration_object.hpp"

#include <graphics.hpp>

chaser_object::chaser_object() {
	animation.fps = 15.0f;
	side_direction = ne::direction_side::right;
}

void chaser_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	ne::game_object::update(world, chunk);
	bool right = (side_direction == ne::direction_side::right);
	bool left = !right;
	auto move = component<game_object_move_component>();
	move->is_running = true;
	world->each<player_object>([&](auto player) {
		if (player->side_direction == ne::direction_side::right) {
			return;
		}
		auto player_move = player->component<game_object_move_component>();
		if (player_move->is_jumping()) {
			move->jump();
		}
	});
	auto col_transform = collision_transform();
	col_transform.position.x += 20.0f * (right ? 1.0f : -1.0f);
	world->each_if<decoration_object>([&](auto decoration) {
		if (decoration->collision_transform().collides_with(col_transform)) {
			move->jump();
			return true;
		}
		return false;
	});
	bool moved = move->move(world, left, right);
	if (!moved) {
		move->jump();
	}
}

void chaser_object::draw() {
	auto move = component<game_object_move_component>();
	ne::texture* sprite = &textures.objects.chaser.idle[direction];
	if (move->is_running) {
		sprite = &textures.objects.chaser.run[direction];
	}
	if (move->is_jumping()) {
		sprite = &textures.objects.chaser.jump[direction];
	}
	animated_quad().bind();
	ne::shader::set_transform(&transform);
	sprite->bind();
	animation.draw(true);
}

void chaser_object::write(ne::memory_buffer* buffer) {
	ne::game_object::write(buffer);
}

void chaser_object::read(ne::memory_buffer* buffer) {
	ne::game_object::read(buffer);
}
