#pragma once

#include "game_object_types.hpp"

#include <transform.hpp>
#include <graphics.hpp>
#include <memory.hpp>

class game_state;
class game_world;

class game_object {
public:

	ne::transform3f transform;
	int direction = 1;

	ne::sprite_animation animation;

	game_object(game_state* game) : game(game) {}
	virtual ~game_object() = default;

	virtual game_object_type type() const {
		return game_object_type::any;
	}

	virtual void update(game_world* world) = 0;
	virtual void draw() = 0;
	virtual void write(ne::memory_buffer* buffer);
	virtual void read(ne::memory_buffer* buffer);

	void apply_gravity(game_world* world);

protected:

	game_state* game = nullptr;

};
