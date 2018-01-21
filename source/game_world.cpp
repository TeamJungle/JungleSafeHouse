#include "game_world.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"
#include "assets.hpp"

#include <graphics.hpp>
#include <camera.hpp>

void game_world_terrain::draw() {
	area.position.xy = { -1000.0f, 0.f };
	area.scale.xy = { 10000.0f, 2000.0f };
	ne::shader::set_transform(&area);
	ne::set_drawing_shape(0);
	ne::shader::set_color(0.2f, 0.8f, 0.3f, 1.0f);
	textures.blank.bind();
	ne::draw_vertices();
}

game_world::game_world(game_state* game) : objects(game) {

}

void game_world::update() {
	objects.update(this);

	bool player_is_dead = !objects.each<player_object>([&](player_object* player) -> bool {
		return objects.each<chaser_object>([&](chaser_object* chaser) -> bool {
			if (player->transform.collides_with(chaser->transform)) {
				return false;
			}
			return true;
		});
	});
	if (player_is_dead) {
		ne::ortho_camera::bound()->target = &objects.find_first_of_type<chaser_object>()->transform;
		objects.destroy(0);
	}
}

void game_world::draw(const ne::transform3f& view) {
	terrain.draw();
	ne::shader::set_color(1.0f, 1.0f, 1.0f, 1.0f);
	objects.draw();
}
