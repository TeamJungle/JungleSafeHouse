#pragma once

#include "menu.hpp"

class pause_menu {
public:

	bool active = false;

	pause_menu();

	void update(const ne::vector2f& position);
	void draw(const ne::transform3f& view);

private:

	basic_menu menu;

};
