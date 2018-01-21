#pragma once

#include "game_object.hpp"

class chaser_object : public game_object {
public:

	bool is_running = false;

	chaser_object(game_state * game);
	~chaser_object() override;

	static game_object_type Type() {
		return game_object_type::chaser;
	}

	game_object_type type() const override {
		return Type();
	}

	virtual void update(game_world* world) override;
	virtual void draw() override;
	virtual void write(ne::memory_buffer* buffer) override;
	virtual void read(ne::memory_buffer* buffer) override;

};
