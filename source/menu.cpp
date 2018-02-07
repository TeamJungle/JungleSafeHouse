#include "menu.hpp"
#include "assets.hpp"
#include "game.hpp"

 menu_state::menu_state() {
	 add_button("Play", [] {
		 ne::swap_state<game_state>();
	 });
	 add_button("Settings", [] {
		
	 });
	 add_button("Quit", [] {
		 std::exit(ne::stop_engine(0));
	 });
}

 menu_state::~menu_state() {

}

void menu_state::update() {
	camera.update();
	float margin_y = camera.transform.scale.height / 2.0f - (float)textures.bg.menu.size.height / 2.0f;
	for (auto& i : buttons) {
		i.transform.position.xy = {
			camera.transform.scale.width / 2.0f - i.transform.scale.width / 2.0f,
			margin_y + i.transform.scale.height * 1.3f
		};
		i.update();
		if (i.transform.collides_with(ne::mouse_position_f())) {
			i.label_color = { 0.7f, 1.0f, 0.5f };
		} else {
			i.label_color = 1.0f;
		}
		margin_y += i.transform.scale.height * 1.3f;
	}
}

void menu_state::draw() {
	camera.transform.scale.xy = ne::window_size_f();

	shaders.basic.bind();
	camera.bind();

	still_quad().bind();
	ne::shader::set_color(1.0f);
	textures.bg.menu.bind();
	ne::transform3f bg;
	bg.scale.xy = textures.bg.menu.size.to<float>() * 2.0f;
	ne::shader::set_transform(&bg);
	still_quad().draw();

	textures.bg.popup.bind();
	bg.scale.xy = textures.bg.popup.size.to<float>() / 2.0f;
	bg.position.x = camera.transform.scale.width / 2.0f - bg.scale.width / 2.0f;
	bg.position.y = camera.transform.scale.height / 2.0f - bg.scale.height / 2.0f;
	ne::shader::set_transform(&bg);
	still_quad().draw();

	for (auto& i : buttons) {
		i.draw();
	}
}

void menu_state::add_button(const std::string& text, const std::function<void()>& action) {
	buttons.emplace_back(ne::ui_button());
	buttons.back().label.font = &fonts.hud;
	buttons.back().label.render(text);
	buttons.back().sprite = &textures.button;
	buttons.back().transform.scale.width = textures.button.size.width / 6.0f;
	buttons.back().transform.scale.height = textures.button.size.height / 2.0f;
	buttons.back().button_shape = &animated_quad();
	buttons.back().label_shape = &still_quad();
	buttons.back().click.listen(action);
}
