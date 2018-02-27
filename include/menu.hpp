#pragma once

#include "button_menu.hpp"

#include <engine.hpp>
#include <camera.hpp>
#include <ui.hpp>

class menu_state : public ne::program_state {
public:

	ne::ortho_camera camera;

	menu_state();
	~menu_state() override;

	void update() override;
	void draw() override;

private:

	button_menu menu;

};

