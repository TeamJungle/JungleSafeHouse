#include "world.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"
#include "decoration_object.hpp"
#include "slide_under_object.hpp"
#include "assets.hpp"
#include "game.hpp"
#include "move.hpp"

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
	define<slide_under_object>(SLIDE_UNDER_TREE, "Tree (Slide under)", 0);
}

void game_world::generate() {
	for (int i = 0; i < 1; i++) {
		place_chunk(i, 0);
	}

	spawn(definitions.objects.meta->get_meta(OBJECT_TYPE_PLAYER, 0), { 800.0f, 500.0f });
	spawn(definitions.objects.meta->get_meta(OBJECT_TYPE_CHASER, CHASER_TIGER), { 100.0f, 500.0f });

	spawn(definitions.objects.meta->get_meta(OBJECT_TYPE_SLIDE_UNDER, 0), { 1500.0f, 450.0f });
}

game_world::game_world() {
	definitions.objects.meta = new game_object_definitions();
	definitions.objects.meta->initialize();

	generate();

	// Configure backgrounds
	backgrounds.background.top_offset.y = 200.0f;
	backgrounds.trees.top_offset.y = 250.0f;
	backgrounds.mid.top_offset.y = 200.0f;

	// Configure foregrounds
	backgrounds.bottom.zoom = 1.0f;
	backgrounds.bottom.top_offset.y = 400.0f;
	backgrounds.bottom.bottom_offset.y = 192.0f * 0.75f;

	backgrounds.fog_back.top_offset.y = 250.0f;
	backgrounds.fog_back.speed = 500.0f;
	backgrounds.fog_back.timer.start();

	backgrounds.fog_front.zoom = 0.5f;
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
	player_object* dead_player = nullptr;
	each<player_object>([&](player_object* player) {
		each<chaser_object>([&](chaser_object* chaser) {
			if (player->transform.collides_with(chaser->transform)) {
				dead_player = player;
				ne::ortho_camera::bound()->target = &chaser->transform;
			}
		});
	});
	if (dead_player) {
		destroy_object(dead_player->id, nullptr);
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
