#pragma once

#include "button_menu.hpp"

class pause_menu {
public:

	pause_menu();

	void update(const ne::vector2f& camera_size);
	void draw(const ne::vector2f& camera_size);

	bool is_open() const;
	void open();
	void close();

private:

	bool active = false;
	button_menu menu;

};
