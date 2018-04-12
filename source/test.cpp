#include "test.hpp"
#include "assets.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"
#include "menu.hpp"

#include <graphics.hpp>
#include <platform.hpp>

test_state::test_state(bool mode) : mode(mode) {
	camera.target_chase_aspect.y = 1.75f;
	camera.target_chase_speed = { 0.15f, 0.025f };
	camera.zoom = 2.0f;
	load(1);
}

test_state::~test_state() {
	
}

void test_state::update() {
	camera.transform.scale.xy = ne::window_size().to<float>();
	camera.update();
	bool next_level = false;
	auto test = [&] {
		world.update();
		world.each<chaser_object>([&](auto chaser) {
			auto door = world.first<door_object>();
			if (door && chaser->transform.position.x > door->transform.position.x) {
				NE_INFO_X(1, "Completed level " << world.level_num << " in " << timer.seconds() << " seconds");
				next_level = true;
			}
			camera.target = &chaser->transform;
		});
	};
	if (mode == 0) {
		while (120 > timer.seconds()) {
			test();
			if (next_level) {
				break;
			}
		}
	} else {
		test();
	}
	if (timer.seconds() > 120) {
		NE_ERROR_X(1, "Failed to complete level " << world.level_num << " in two minutes");
		next_level = true;
	}
	if (next_level) {
		load(world.level_num + 1);
	}
	if (world.level_num < 1) {
		ne::swap_state<menu_state>();
	}
}

void test_state::draw() {
	ne::shader::set_color(1.0f);
	camera.bind();
	ne::transform3f view;
	view.position.xy = camera.xy();
	view.scale.xy = camera.size();
	world.draw(view);
}

void test_state::load(int num) {
	if (num > 999) {
		world.reset();
		NE_INFO_X(1, "No more levels to test.");
		return;
	}
#if _DEBUG
	std::string path = STRING("worlds/local/" << num << ".world");
#else
	std::string path = STRING("worlds/" << num << ".world");
#endif
	if (!ne::file_exists(path)) {
		load(num + 1);
		return;
	}
	world.load(path);
	if (world.level_num < 1) {
		load(num + 1);
		return;
	}
	auto chaser = world.first<chaser_object>();
	if (!chaser) {
		NE_WARNING_X(1, "Level " << num << " has no tigers. Skipping test.");
		load(num + 1);
		return;
	}
	auto door = world.first<door_object>();
	if (!door || door->leads_to_level_num != 0) {
		NE_ERROR_X(1, "Level " << num << " has no exit door");
		load(num + 1);
		return;
	}
	auto player = world.first<player_object>();
	while (player) {
		world.destroy_object(player->id, nullptr);
		player = world.first<player_object>();
	}
	timer.start();
}
