#include "pause.hpp"
#include "assets.hpp"

pause_menu::pause_menu() {
	menu.font = &fonts.hud;
	menu.add_button("Resume", [&] {
		active = false;
	});
	menu.add_button("Settings", [] {

	});
	menu.add_button("Main Menu", [] {
		ne::swap_state<menu_state>();
	});
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
	menu.draw(camera_size, 0.0f, textures.bg.popup.size.to<float>() / 2.0f);
}
