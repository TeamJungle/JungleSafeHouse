#pragma once

#include <ui.hpp>

class button_menu {
public:

	ne::vector2f button_downscale = { 6.0f, 2.0f };
	ne::font* font = nullptr;
	bool has_background = true;

	void update(const ne::vector2f& camera_size, const ne::vector2f& position, const ne::vector2f& size);
	void draw(const ne::vector2f& camera_size, const ne::vector2f& position, const ne::vector2f& size);

	void add_button(const std::string& text, const std::function<void()>& action);

private:

	std::vector<ne::ui_button> buttons;

};
