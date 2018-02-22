#pragma once

#include <engine.hpp>
#include <camera.hpp>
#include <ui.hpp>

class basic_menu {
public:

	ne::vector2f button_downscale = { 6.0f, 2.0f };
	ne::font* font = nullptr;

	void update(const ne::vector2f& camera_size, const ne::vector2f& position, const ne::vector2f& size);
	void draw(const ne::vector2f& camera_size, const ne::vector2f& position, const ne::vector2f& size);

	void add_button(const std::string& text, const std::function<void()>& action);

private:
	
	std::vector<ne::ui_button> buttons;

};

class menu_state : public ne::program_state {
public:

	ne::ortho_camera camera;

	menu_state();
	~menu_state() override;

	void update() override;
	void draw() override;

private:

	basic_menu menu;

};

