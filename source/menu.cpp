#include "menu.hpp"
#include "assets.hpp"
#include "game.hpp"
#include "editor.hpp"

 menu_state::menu_state() {
	menu.font = &fonts.hud;
	menu.add_button("Play", [] {
		ne::swap_state<game_state>();
	});
	menu.add_button("Editor", [] {
		ne::swap_state<editor_state>();
	});
	menu.add_button("Quit", [] {
		std::exit(ne::stop_engine(0));
	});
	ne::set_music_volume(15);
	//music.safehouse.play();
}

 menu_state::~menu_state() {

}

 void menu_state::update() {
	camera.transform.scale.xy = ne::window_size().to<float>();
	camera.update();
	camera.bind();
	menu.update(camera.size(), 0.0f, textures.bg.menu.size.to<float>());
 }

 void menu_state::draw() {
	shaders.basic.bind();
	camera.bind();
	textures.bg.menu.bind();
	ne::transform3f bg;
	bg.scale.xy = textures.bg.menu.size.to<float>() * 2.0f;
	ne::shader::set_transform(&bg);
	ne::shader::set_color(1.0f);
	still_quad().draw();
	menu.draw(camera.size(), 0.0f, textures.bg.popup.size.to<float>() / 2.0f);
 }
