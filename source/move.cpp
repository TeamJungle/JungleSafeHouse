#include "move.hpp"
#include "world.hpp"
#include "platform_object.hpp"
#include "decoration_object.hpp"

void game_object_move_component::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	apply_gravity(world);
	if (is_sliding) {
		if (slide_left > 0.0f) {
			move(world, false, true);
			slide_left -= 6.0f;
			if (slide_left < 1.0f) {
				slide_left = 0.0f;
				is_sliding = false;
			}
		} else {
			move(world, true, false);
			slide_left += 6.0f;
			if (slide_left > -1.0f) {
				slide_left = 0.0f;
				is_sliding = false;
			}
		}
	}
}

void game_object_move_component::apply_gravity(ne::game_world* world) {
	bool not_collided = false;
	if (jump_force > 0.0f) {
		bool not_collided = world->each_if<platform_object>([&](auto platform) {
			ne::transform3f t = parent->collision_transform();
			t.position.y -= jump_force;
			return !platform->transform.collides_with(t);
		});
		if (not_collided) {
			not_collided = world->each_if<decoration_object>([&](auto platform) {
				if (platform->collision_transform().scale.width < 1.0f) {
					return true;
				}
				ne::transform3f t = parent->collision_transform();
				t.position.y -= jump_force;
				return !platform->collision_transform().collides_with(t);
			});
		}
		if (not_collided) {
			parent->transform.position.y -= jump_force;
			jump_force -= 0.2f;
			if (jump_force < 0.0f) {
				jump_force = 0.0f;
			}
		}
	}
	int limit = 0;
	const float ground_y = ((game_world*)world)->ground_y;
	not_collided = world->each_if<platform_object>([&](auto platform) {
		ne::transform3f t = parent->collision_transform();
		t.position.y += 4.0f;
		return !platform->transform.collides_with(t);
	});
	if (not_collided) {
		not_collided = world->each_if<decoration_object>([&](auto platform) {
			if (platform->collision_transform().scale.width < 1.0f) {
				return true;
			}
			ne::transform3f t = parent->collision_transform();
			t.position.y += 4.0f;
			return !platform->collision_transform().collides_with(t);
		});
	}
	while (not_collided && limit < 5 && parent->transform.position.y + parent->transform.scale.y < ground_y) {
		parent->transform.position.y += 1.0f;
		limit++;
	}
	if (limit == 0) {
		current_jumps = 0;
	}
}

void game_object_move_component::jump() {
	if (is_sliding) {
		return;
	}
	// NOTE: Make this check more flexible in the future to allow for multijumps.
	if (current_jumps > 1) {
		return;
	}
	current_jumps++;
	jump_force = max_jump_force;
}

void game_object_move_component::slide(int direction) {
	if (is_sliding) {
		return;
	}
	if (current_jumps > 0) {
		return;
	}
	is_sliding = true;
	slide_left = 350.0f * (direction == 1 ? 1.0f : -1.0f);
}

void game_object_move_component::move(ne::game_world* world, bool left, bool right) {
	is_running = false;
	if (right && left) {
		return;
	}
	if (!right && !left) {
		if (speed > 0.0f) {
			speed -= acceleration * 5.0f;
			if (parent->side_direction == ne::direction_side::right) {
				right = true;
			} else if (parent->side_direction == ne::direction_side::left) {
				left = true;
			}
		} else {
			return;
		}
	}
	is_running = true;
	if (speed < 5.0f) {
		speed += acceleration;
	}
	bool not_collided = world->each_if<platform_object>([&](auto platform) {
		ne::transform3f t = parent->collision_transform();
		t.position.y -= 2.0f;
		if (right) {
			t.position.x += speed;
		} else {
			t.position.x -= speed;
		}
		return !platform->transform.collides_with(t);
	});
	if (not_collided) {
		not_collided = world->each_if<decoration_object>([&](auto platform) {
			if (platform->collision_transform().scale.width < 1.0f) {
				return true;
			}
			ne::transform3f t = parent->collision_transform();
			t.position.y -= 2.0f;
			if (right) {
				t.position.x += speed;
			} else {
				t.position.x -= speed;
			}
			return !platform->collision_transform().collides_with(t);
		});
	}
	if (right) {
		if (not_collided) {
			parent->transform.position.x += speed;
		}
		parent->side_direction = ne::direction_side::right;
	}
	if (left) {
		if (not_collided) {
			parent->transform.position.x -= speed;
		}
		parent->side_direction = ne::direction_side::left;
	}
}

bool game_object_move_component::is_jumping() const {
	return current_jumps > 0;
}

void game_object_move_component::stop() {
	is_running = false;
	speed = -1.0f;
}
