#pragma once

#include <ui.hpp>

class button_menu {
public:

	ne::vector2f button_downscale = { 6.0f, 2.0f };
	ne::font* font = nullptr;
	bool has_background = true;
	bool is_horizontal = false;

	void update(const ne::vector2f& camera_size, const ne::vector2f& position, const ne::vector2f& size);
	void draw(const ne::vector2f& camera_size, const ne::vector2f& position, const ne::vector2f& size);

	int add_button(const std::string& text, const std::function<void()>& action);

	void disable();
	void enable();

	void set_active(int index);

private:

	std::vector<ne::ui_button> buttons;
	int active_index = -1;

};
