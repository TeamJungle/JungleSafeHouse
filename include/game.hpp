#pragma once

#include "game_input.hpp"
#include "world.hpp"
#include "pause.hpp"

#include <engine.hpp>
#include <camera.hpp>
#include <ui.hpp>

class player_object;

class game_state : public ne::program_state {
public:

	pause_menu pause;
	
	ne::ortho_camera camera;
	game_world world;

	ne::font_text fps_label;

	game_state();
	~game_state() override;

	void update() override;
	void draw() override;

};

ne::drawing_shape& still_quad();
ne::drawing_shape& animated_quad();
