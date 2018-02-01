#pragma once

#include <engine.hpp>
#include <camera.hpp>
#include <ui.hpp>

class menu_state : public ne::program_state {
public:

	ne::ortho_camera camera;
	std::vector<ne::ui_button> buttons;

	menu_state();
	~menu_state() override;

	void update() override;
	void draw() override;
	void add_button(const std::string& text, const std::function<void()>& action);
};

