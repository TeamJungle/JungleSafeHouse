#include "pause.hpp"
#include "assets.hpp"
#include "editor.hpp"
#include "menu.hpp"

pause_menu::pause_menu() {
	menu.font = &fonts.hud;
	menu.add_button("Resume", [&] {
		close();
	});
	menu.add_button("Editor", [] {
		ne::swap_state<editor_state>();
	});
	menu.add_button("Main Menu", [] {
		ne::swap_state<menu_state>();
	});
	close();
}

void pause_menu::update(const ne::vector2f& camera_size) {
	if (!active) {
		return;
	}
	menu.update(camera_size, 0.0f, textures.bg.menu.size.to<float>());
}

void pause_menu::draw(const ne::vector2f& camera_size) {
	if (!active) {
		return;
	}
	menu.draw(camera_size, 0.0f, textures.ui.popup.size.to<float>() / 2.0f);
}

bool pause_menu::is_open() const {
	return active;
}

void pause_menu::open() {
	menu.enable();
	active = true;
}

void pause_menu::close() {
	menu.disable();
	active = false;
}
