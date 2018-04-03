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
	game_world* w = (game_world*)world;
	save_data = w->save_data;
	auto move = component<game_object_move_component>();
	if (up_hit == -1) {
		up_hit = input().up_hit.listen([this, w] {
			if (!w->shop.is_open) {
				component<game_object_move_component>()->jump();
			}
		});
	}
	if (down_hit == -1) {
		down_hit = input().down_hit.listen([this, w] {
			if (!w->shop.is_open) {
				component<game_object_move_component>()->slide(direction);
			}
		});
	}
	if (interact_hit == -1) {
		interact_hit = input().interact.listen([this, w, move] {
			if (w->shop.is_open) {
				return;
			}
			bool no_action = w->each_if<npc_object>([&](auto npc) {
				if (!collision_transform().collides_with(npc->transform)) {
					return true;
				}
				if (!w->shop.is_open) {
					npc->open_shop(&w->shop);
					move->stop();
				}
				return false;
			});
			if (!no_action || w->shop.is_open) {
				return;
			}
			no_action = w->each_if<door_object>([&](auto door) {
				if (!collision_transform().collides_with(door->transform)) {
					return true;
				}
				if (door->is_open) {
					w->save_data->complete_level(w->level_num);
					w->change(door->leads_to_level_num);
					return false;
				}
				if (door->opening_cost > w->save_data->get_coins()) {
					return false;
				}
				door->is_open = true;
				w->save_data->unlock_level(door->leads_to_level_num);
				w->save_data->add_coins(-door->opening_cost);
				settings::play(&audio.door, 0.15f);
				return false;
			});
			if (!no_action) {
				return;
			}
			if (w->save_data->has_machete()) {
				w->each<vine_object>([&](auto vine) {
					if (!collision_transform().collides_with(vine->transform)) {
						return;
					}
					vine->health -= 0.4f;
					settings::play(&audio.vine[vine_sound++], 0.05f);
					if (vine_sound > 2) {
						vine_sound = 0;
					}
				});
			}
		});
	}
	if (w->shop.is_open) {
		return;
	}
	move->max_speed = 5.0f;
	w->each_if<vine_object>([&](auto vine) {
		if (!collision_transform().collides_with(vine->transform)) {
			return true;
		}
		move->max_speed = 2.0f;
		return false;
	});
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
	ne::texture* machete = &textures.objects.machete.idle[direction];
	int old_state = state;
	state = 0;
	if (move->is_running) {
		sprite = &textures.objects.player.run[direction];
		machete = &textures.objects.machete.run[direction];
		animation.fps = 30.0f;
		state = 1;
	}
	if (move->is_sliding) {
		sprite = &textures.objects.player.slide[direction];
		machete = &textures.objects.machete.slide[direction];
		animation.fps = 14.0f;
		state = 2;
		// A bit hackish solution to prevent the animation from looping.
		if (animation.frame > 7) {
			animation.frame = 7;
			animation.sub_frame = 7.0f;
		}
	}
	if (move->is_jumping()) {
		sprite = &textures.objects.player.jump[direction];
		machete = &textures.objects.machete.jump[direction];
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
	if (!save_data) {
		return;
	}
	if (save_data->has_machete()) {
		machete->bind();
		animation.draw(false);
	}
}

void player_object::write(ne::memory_buffer* buffer) {
	ne::game_object::write(buffer);
}

void player_object::read(ne::memory_buffer* buffer) {
	ne::game_object::read(buffer);
}
