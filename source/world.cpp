#include "world.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"
#include "terrain_object.hpp"
#include "assets.hpp"
#include "game.hpp"

#include <graphics.hpp>
#include <camera.hpp>

// TODO: Relocate this function. Soon this class won't exist anyway.
void game_object_definitions::initialize() {
	meta.insert(meta.begin(), TOTAL_OBJECT_TYPES, {});
	define<player_object, 0>("Player", 0);
	define<chaser_object, 0>("Chaser", 0);
	define<terrain_object, 0>("Terrain", 0);
}

game_world::game_world() {
	definitions.objects.meta = new game_object_definitions();
	definitions.objects.meta->initialize();

	// NOTE: This world generation code is very temporary.

	// Place chunks for objects to be spawned on.
	for (int i = 0; i < 50; i++) {
		place_chunk(i, 0);
		place_chunk(i, 1);
	}

	// Spawn player and chaser.
	spawn(definitions.objects.meta->get_meta(OBJECT_TYPE_PLAYER, 0), { 800.0f, 300.0f });
	spawn(definitions.objects.meta->get_meta(OBJECT_TYPE_CHASER, 0), { 100.0f, 300.0f });

	// Spawn the terrain objects.
	for (int i = 0; i < 50; i++) {
		float x = 1.0f + (float)i * (float)ne::game_world_chunk::pixel_width();
		spawn(definitions.objects.meta->get_meta(OBJECT_TYPE_TERRAIN, 0), { x, 600.0f });
	}
}

void game_world::update() {
	update_objects();

	// Kill all players colliding with a chaser.
	bool player_is_dead = false;
	each<player_object>([&](player_object* player) {
		each<chaser_object>([&](chaser_object* chaser) {
			if (player->transform.collides_with(chaser->transform)) {
				player_is_dead = true;
			}
		});
	});
	if (player_is_dead) {
		auto chaser = first<chaser_object>();
		ne::ortho_camera::bound()->target = &chaser->transform;
		destroy_object(chaser->id, nullptr);
	}
}

void game_world::draw(const ne::transform3f& view) {
	each<ne::game_object>([&](ne::game_object* object) {
		object->draw();
	});
}
