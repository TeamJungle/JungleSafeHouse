#include "game.hpp"
#include "assets.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"

#include <graphics.hpp>

game_state::game_state() {
	camera.target_chase_aspect.y = 1.75f;
	camera.target_chase_speed = { 0.15f, 0.025f };
	camera.zoom = 2.0f;
}

game_state::~game_state() {

}

void game_state::update() {
	auto player = world.first<player_object>();
	if (player) {
		camera.target = &player->transform;
	}
	camera.update();
	world.update();
	fps_label.font = &fonts.debug;
	fps_label.render(STRING("Delta " << ne::delta() << "\nFPS: " << ne::current_fps()));
}

void game_state::draw() {
	camera.bind();
	camera.transform.scale.xy = ne::window_size_f();

	ne::transform3f view;
	view.position.xy = camera.xy();
	view.scale.xy = camera.size();

	ne::shader::set_color(1.0f);
	world.draw(view);

	ne::shader::set_color(1.0f);
	fps_label.transform.position = {
		view.position.x + view.scale.width - 128.0f,
		view.position.y + 4.0f,
		0.0f
	};
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
