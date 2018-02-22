#include "game.hpp"
#include "assets.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"

#include <graphics.hpp>

game_state::game_state() {
	camera.target_chase_aspect.y = 1.75f;
	camera.target_chase_speed = { 0.15f, 0.025f };
	camera.zoom = 2.0f;
	input().pause.listen([this] {
		pause.active = !pause.active;
	});
}

game_state::~game_state() {

}

void game_state::update() {
	camera.transform.scale.xy = ne::window_size_f();
	ui_camera.transform.scale.xy = ne::window_size_f();
	auto player = world.first<player_object>();
	if (player) {
		camera.target = &player->transform;
	}
	camera.update();
	pause.update(ui_camera.size());
	if (!pause.active) {
		world.update();
	}
	fps_label.font = &fonts.debug;
	fps_label.render(STRING("Delta " << ne::delta() << "\nFPS: " << ne::current_fps()));
}

void game_state::draw() {
	camera.bind();

	ne::transform3f view;
	view.position.xy = camera.xy();
	view.scale.xy = camera.size();

	ne::shader::set_color(1.0f);
	world.draw(view);

	ui_camera.bind();
	pause.draw(ui_camera.size());

	ne::shader::set_color(1.0f);
	fps_label.transform.position.xy = { ui_camera.width() - 128.0f, 4.0f };
	still_quad().bind();
	fps_label.draw();
}

ne::drawing_shape& still_quad() {
	static ne::drawing_shape shape;
	return shape;
}

ne::drawing_shape& animated_quad() {
	static ne::drawing_shape shape;
	return shape;
}
