#include "world.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"
#include "decoration_object.hpp"
#include "assets.hpp"
#include "game.hpp"

#include <graphics.hpp>
#include <camera.hpp>

// TODO: Relocate this function. Soon this class won't exist anyway.
void game_object_definitions::initialize() {
	meta.insert(meta.begin(), TOTAL_OBJECT_TYPES, {});
	define<player_object>(0, "Player", 0);
	define<chaser_object>(CHASER_TIGER, "Tiger", 0);
	for (int i = 0; i < TOTAL_DECORATIONS; i++) {
		define<decoration_object>(i, STRING("Decoration " << i), 0);
	}
}

game_world::game_world() {
	definitions.objects.meta = new game_object_definitions();
	definitions.objects.meta->initialize();

	// NOTE: This world generation code is very temporary.

	// Place chunks for objects to be spawned on.
	for (int i = 0; i < 10; i++) {
		place_chunk(i, 0);
		place_chunk(i, 1);
	}

	// Spawn player and chaser.
	spawn(definitions.objects.meta->get_meta(OBJECT_TYPE_PLAYER, 0), { 800.0f, 300.0f });
	spawn(definitions.objects.meta->get_meta(OBJECT_TYPE_CHASER, CHASER_TIGER), { 100.0f, 300.0f });

	// Spawn random decorations.
	for (int i = 0; i < 10; i++) {
		float x = (float)i * (float)ne::game_world_chunk::pixel_width() / 2.0f;
		//spawn(definitions.objects.meta->get_meta(OBJECT_TYPE_DECORATION, i % 6), { x, 500.0f });
	}

	// Spawn ground
	for (int i = 0; i < 10; i++) {
		float x = (float)i * (float)ne::game_world_chunk::pixel_width() / 10.0f;
		//spawn(definitions.objects.meta->get_meta(OBJECT_TYPE_DECORATION, 8 + i % 10), { x, 560.0f });
	}


	// Configure backgrounds
	backgrounds.background.zoom = 0.5f;
	backgrounds.background.top_offset.y = 200.0f;

	backgrounds.trees.zoom = 0.5f;
	backgrounds.trees.top_offset.y = 250.0f;

	backgrounds.mid.zoom = 0.5f;
	backgrounds.mid.top_offset.y = 200.0f;

	// Configure foregrounds
	backgrounds.bottom.zoom = 0.5f;
	backgrounds.bottom.top_offset.y = 300.0f;
	backgrounds.bottom.bottom_offset.y = 180.0f;

	backgrounds.fog_back.zoom = 0.3f;
	backgrounds.fog_back.top_offset.y = 250.0f;
	backgrounds.fog_back.speed = 500.0f;
	backgrounds.fog_back.timer.start();

	backgrounds.fog_front.zoom = 0.15f;
	backgrounds.fog_front.top_offset.x = 100.0f;
	backgrounds.fog_front.top_offset.y = 400.0f;
	backgrounds.fog_front.speed = 1500.0f;
	backgrounds.fog_front.timer.start();

	backgrounds.top.zoom = 0.75f;
	backgrounds.top.top_offset.y = 200.0f;

	backgrounds.top_lines.zoom = 0.75f;
	backgrounds.top_lines.top_offset.y = 200.0f;
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
	// Draw backgrounds.
	backgrounds.background.draw(view, &textures.bg.bg);
	backgrounds.trees.draw(view, &textures.bg.bg_back);
	backgrounds.fog_back.draw(view, &textures.bg.bg_fog);
	backgrounds.mid.draw(view, &textures.bg.bg_mid);

	// Draw objects.
	each<ne::game_object>([&](ne::game_object* object) {
		object->draw();
	});

	// Draw foregrounds
	backgrounds.bottom.draw(view, &textures.bg.bg_bott);
	backgrounds.top_lines.draw(view, &textures.bg.bg_top_lines);
	backgrounds.top.draw(view, &textures.bg.bg_top);
	backgrounds.fog_front.draw(view, &textures.bg.bg_fog);
}
