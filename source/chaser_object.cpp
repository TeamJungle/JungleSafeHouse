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
	collision.offset = { 15, 20 };
	collision.size = { 80, 30 };
}

void chaser_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	ne::game_object::update(world, chunk);
	bool right = (side_direction == ne::direction_side::right);
	bool left = !right;
	auto move = component<game_object_move_component>();
	auto this_collision = collision_transform();
	move->is_running = true;
	move->max_jump_force = 12.0f;
	auto player = world->first<player_object>();
	if (player) {
		auto player_move = player->component<game_object_move_component>();
		auto player_collision = player->collision_transform();
		if (player->side_direction == ne::direction_side::left && player_move->is_jumping()) {
			move->jump();
			settings::play(&audio.tiger, 0.15f);
		}
		if (player_collision.position.x + player_collision.scale.width > this_collision.position.x) {
			side_direction = ne::direction_side::right;
			right = true;
			left = false;
		} else {
			side_direction = ne::direction_side::left;
			right = false;
			left = true;
		}
	}
	this_collision.position.x += 30.0f * (right ? 1.0f : -1.0f);
	world->each_if<decoration_object>([&](auto decoration) {
		if (decoration->collision_transform().collides_with(this_collision)) {
			move->jump();
			return true;
		}
		return false;
	});
	if (settings::difficulty == DIFFICULTY_EASY) {
		move->max_speed = 4.0f;
	} else if (settings::difficulty == DIFFICULTY_NORMAL) {
		move->max_speed = 5.0f;
	} else if (settings::difficulty == DIFFICULTY_HARD) {
		move->max_speed = 6.0f;
	}
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
