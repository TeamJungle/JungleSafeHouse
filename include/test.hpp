#pragma once

#include "world.hpp"

#include <engine.hpp>

class test_state : public ne::program_state {
public:

	ne::ortho_camera camera;
	game_world world;
	ne::timer timer;
	bool mode = 0;

	test_state(bool mode);
	~test_state() override;

	void update() override;
	void draw() override;

private:
	
	void load(int num);

};

