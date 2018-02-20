#include "menu.hpp"
#include "assets.hpp"
#include "game.hpp"

 menu_state::menu_state() {
	 menu.font = &fonts.hud;
	 menu.add_button("Play", [] {
		 ne::swap_state<game_state>();
	 });
	 menu.add_button("Settings", [] {
		
	 });
	 menu.add_button("Quit", [] {
		 std::exit(ne::stop_engine(0));
	 });
}

 menu_state::~menu_state() {

}

 void menu_state::update() {
	 camera.update();
	 camera.bind();
	 menu.update(0.0f, textures.bg.menu.size.to<float>());
 }

 void menu_state::draw() {
	 camera.transform.scale.xy = ne::window_size_f();
	 shaders.basic.bind();
	 camera.bind();
	 textures.bg.menu.bind();
	 ne::transform3f bg;
	 bg.scale.xy = textures.bg.menu.size.to<float>() * 2.0f;
	 ne::shader::set_transform(&bg);
	 ne::shader::set_color(1.0f);
	 still_quad().draw();
	 menu.draw(0.0f, textures.bg.popup.size.to<float>() / 2.0f);
 }

void basic_menu::update(const ne::vector2f& position, const ne::vector2f& size) {
	ne::ortho_camera* camera = ne::ortho_camera::bound();
	ne::vector2f camera_size = camera->size();
	float margin_y = camera_size.height / 2.0f - size.y / 2.0f;
	for (auto& i : buttons) {
		i.transform.position.xy = {
			position.x + camera_size.width / 2.0f - i.transform.scale.width / 2.0f,
			position.y + margin_y + i.transform.scale.height * 1.3f
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

void basic_menu::draw(const ne::vector2f& position, const ne::vector2f& size) {
	ne::ortho_camera* camera = ne::ortho_camera::bound();
	ne::vector2f camera_size = camera->size();
	still_quad().bind();
	ne::shader::set_color(1.0f);

	textures.bg.popup.bind();
	ne::transform3f bg;
	bg.scale.xy = size;
	bg.position.x = position.x + camera_size.width / 2.0f - bg.scale.width / 2.0f;
	bg.position.y = position.y + camera_size.height / 2.0f - bg.scale.height / 2.0f;
	ne::shader::set_transform(&bg);
	still_quad().draw();

	for (auto& i : buttons) {
		i.draw();
	}
}

void basic_menu::add_button(const std::string& text, const std::function<void()>& action) {
	buttons.emplace_back(ne::ui_button());
	buttons.back().label.font = font;
	buttons.back().label.render(text);
	buttons.back().sprite = &textures.button;
	buttons.back().transform.scale.width = textures.button.size.width / button_downscale.width;
	buttons.back().transform.scale.height = textures.button.size.height / button_downscale.height;
	buttons.back().button_shape = &animated_quad();
	buttons.back().label_shape = &still_quad();
	buttons.back().click.listen(action);
}
