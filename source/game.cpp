#include "game.hpp"
#include "assets.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"

#include <graphics.hpp>

game_state::game_state() : world(this) {
	camera.target_chase_aspect.y = 1.5f;
	camera.target_chase_speed = { 0.2f, 0.2f };
	camera.zoom = 2.0f;

	world.objects.add(new player_object(this));
	auto player = world.objects.find<player_object>(0);
	player->transform.position.y = -200.0f;
	camera.target = &player->transform;

	world.objects.add(new chaser_object(this));
	auto chaser = world.objects.find<chaser_object>(1);
	chaser->transform.position.xy = { -800.0f, -200.0f };
}

game_state::~game_state() {

}

void game_state::update() {
	camera.update();
	world.update();
	fps_label.font = &fonts.hud;
	fps_label.render(STRING("Delta " << ne::delta() << "\nFPS: " << ne::current_fps()));
}

void game_state::draw() {
	camera.bind();
	camera.transform.scale.xy = ne::window_size_f();

	ne::transform3f view;
	view.position.xy = camera.xy();
	view.scale.xy = camera.size();

	world.draw(view);

	ne::shader::set_color(0.7f, 0.7f, 0.7f, 1.0f);
	fps_label.transform.position = {
		view.position.x + view.scale.width - 196.0f,
		view.position.y + 4.0f,
		0.0f
	};
	ne::set_drawing_shape(0);
	fps_label.draw();
}
