#pragma once

#include "menu.hpp"

class pause_menu {
public:

	bool active = false;

	pause_menu();

	void update(const ne::vector2f& camera_size);
	void draw(const ne::vector2f& camera_size);

private:

	basic_menu menu;

};
