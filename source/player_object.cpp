#include "player_object.hpp"
#include "game.hpp"
#include "assets.hpp"
#include "move.hpp"

#include <graphics.hpp>

player_object::player_object() {
	animation.fps = 30.0f;
	side_direction = ne::direction_side::right;
	collision.offset = { 20, 10 };
	collision.size = { 30, 60 };
}

player_object::~player_object() {
	input().up_hit.erase(&up_hit);
	input().down_hit.erase(&down_hit);
	input().interact.erase(&interact_hit);
}

ne::transform3f player_object::collision_transform() {
	ne::transform3f t = ne::game_object::collision_transform();
	auto move = component<game_object_move_component>();
	if (move->is_sliding) {
		float y_duck = t.scale.height * 0.75f;
		t.position.y += y_duck;
		t.scale.height -= y_duck;
	}
	return t;
}

void player_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	ne::game_object::update(world, chunk);
	if (up_hit == -1) {
		up_hit = input().up_hit.listen([this] {
			component<game_object_move_component>()->jump();
		});
	}
	if (down_hit == -1) {
		down_hit = input().down_hit.listen([this] {
			component<game_object_move_component>()->slide(direction);
		});
	}
	if (interact_hit == -1) {
		interact_hit = input().interact.listen([this, world] {
			world->each<door_object>([&](auto door) {
				if (!collision_transform().collides_with(door->transform)) {
					return;
				}
				auto w = (game_world*)world;
				if (door->is_open) {
					w->save_data->complete_level(w->level_num);
					w->change(door->leads_to_level_num);
					return;
				}
				if (door->opening_cost > w->save_data->get_coins()) {
					return;
				}
				door->is_open = true;
				w->save_data->add_coins(-door->opening_cost);
			});
		});
	}
	auto move = component<game_object_move_component>();
	bool up = input().up.is_active();
	bool left = input().left.is_active();
	bool down = input().down.is_active();
	bool right = input().right.is_active();
	if (!move->is_sliding) {
		move->move(world, left, right);
	}
}

void player_object::draw() {
	auto move = component<game_object_move_component>();
	ne::texture* sprite = &textures.objects.player.idle[direction];
	int old_state = state;
	state = 0;
	if (move->is_running) {
		sprite = &textures.objects.player.run[direction];
		animation.fps = 30.0f;
		state = 1;
	}
	if (move->is_sliding) {
		// TODO: Animation should play once (leaving it on the last frame)
		sprite = &textures.objects.player.slide[direction];
		animation.fps = 14.0f;
		state = 2;
	}
	if (move->is_jumping()) {
		sprite = &textures.objects.player.jump[direction];
		animation.fps = 18.0f;
		state = 3;
	}
	if (old_state != state) {
		animation.frame = 0;
		animation.sub_frame = 0.0f;
	}
	animated_quad().bind();
	ne::shader::set_transform(&transform);
	sprite->bind();
	animation.draw(true);
}

void player_object::write(ne::memory_buffer* buffer) {
	ne::game_object::write(buffer);
}

void player_object::read(ne::memory_buffer* buffer) {
	ne::game_object::read(buffer);
}
