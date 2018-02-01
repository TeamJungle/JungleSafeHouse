#include "menu.hpp"
#include "assets.hpp"
#include "game.hpp"

 menu_state::menu_state() {
	 add_button("Start New Game", [] {
		 ne::swap_state<game_state>();
	 });

	 add_button("Quit", [] {
		 std::exit(ne::stop_engine(0));
	 });
}

 menu_state::~menu_state() {

}

void menu_state::update() {
	camera.update();

	ne::vector2f button_size = {
		camera.transform.scale.width / 3.0f,
		camera.transform.scale.height / 12.0f
	};

	float bw = camera.transform.scale.width / 2.0f - button_size.width / 2.0f;
	float margin_y = 50.0f;

	for (auto& i : buttons) {
		i.transform.position.xy = {
			bw,
			button_size.height + margin_y
		};
		i.transform.scale.xy = button_size;
		i.update();
		if (i.transform.collides_with(ne::mouse_position_f())) {
			i.label_color = { 0.0f, 0.0f, 0.0f };
		} else {
			i.label_color = { 0.5f, 0.5f, 0.5f };
		}
		margin_y += button_size.height * 1.3f;
	}
}

void menu_state::draw() {
	camera.transform.scale.xy = ne::window_size_f();

	shaders.basic.bind();
	camera.bind();

	still_quad().bind();
	ne::shader::set_color(0.08f, 0.08f, 0.08f, 1.0f);
	textures.blank.bind();
	ne::transform3f transform = camera.transform;
	transform.position.z = 0.0f;
	ne::shader::set_transform(&transform);
	still_quad().draw();

	for (auto& i : buttons) {
		i.sprite = &textures.blank;
		i.draw();
	}
}

void menu_state::add_button(const std::string& text, const std::function<void()>& action) {
	buttons.emplace_back(ne::ui_button());
	buttons.back().label.font = &fonts.hud;
	buttons.back().label.render(text);
	buttons.back().click.listen(action);
}