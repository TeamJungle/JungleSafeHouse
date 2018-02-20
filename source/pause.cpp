#include "pause.hpp"
#include "assets.hpp"

pause_menu::pause_menu() {
	menu.button_downscale = { 12.0f, 4.0f };
	menu.font = &fonts.hud_small;
	menu.add_button("Resume", [&] {
		active = false;
	});
	menu.add_button("Settings", [] {

	});
	menu.add_button("Quit", [] {
		ne::swap_state<menu_state>();
	});
}

void pause_menu::update(const ne::vector2f& position) {
	if (!active) {
		return;
	}
	menu.update(position, { 0.0f, 250.0f });
}

void pause_menu::draw(const ne::transform3f& view) {
	if (!active) {
		return;
	}
	menu.draw(view.position.xy, { 300.0f, 450.0f });
}
