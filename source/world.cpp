#include "world.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"
#include "decoration_object.hpp"
#include "assets.hpp"
#include "game.hpp"
#include "move.hpp"

#include <graphics.hpp>
#include <camera.hpp>
#include <platform.hpp>

game_world::game_world() {
	definitions.objects.meta = new game_object_definitions();
	definitions.objects.meta->initialize();

	// Place the only chunk we use.
	place_chunk(0, 0);

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

	// Handle updates which may destroy objects.
	each_if<player_object>([&](auto player) {
		// Collect an item if colliding.
		if (save_data) {
			each_if<item_object>([&](auto item) {
				if (!item->collision_transform().collides_with(player->collision_transform())) {
					return true;
				}
				save_data->add_coins(5);
				// TODO: Clean.
				{
					static int last_pickup_sound = 0;
					sounds.pickup[last_pickup_sound].set_volume(1);
					sounds.pickup[last_pickup_sound].play();
					last_pickup_sound++;
					if (last_pickup_sound > 4) {
						last_pickup_sound = 0;
					}
				}
				destroy_object(item->id, nullptr);
				return false;
			});
		}
		// Die if colliding with a chaser.
		return each_if<chaser_object>([&](auto chaser) {
			if (player->collision_transform().collides_with(chaser->collision_transform())) {
				destroy_object(player->id, nullptr);
				return false;
			}
			return true;
		});
	});

	// Change world.
	if (change_to_level_num >= 0) {
		std::string path = STRING("worlds/" << change_to_level_num << ".world");
		if (ne::file_exists(path)) {
			load(path);
		} else {
			ne::show_message("world \"" + path + "\" does not exist.");
		}
		change_to_level_num = -1;
	}
}

void game_world::draw(const ne::transform3f& view) {
	// Backgrounds.
	backgrounds.background.draw(view, &textures.bg.bg);
	backgrounds.trees.draw(view, &textures.bg.bg_back);
	backgrounds.fog_back.draw(view, &textures.bg.bg_fog);
	backgrounds.mid.draw(view, &textures.bg.bg_mid);

	// Objects.
	draw_objects(view);

	// Foregrounds.
	backgrounds.bottom.draw(view, &textures.bg.bg_bott);
	backgrounds.top_lines.draw(view, &textures.bg.bg_top_lines);
	backgrounds.top.draw(view, &textures.bg.bg_top);
	backgrounds.fog_front.draw(view, &textures.bg.bg_fog);

	// Collisions.
	if (draw_collisions) {
		// Ground.
		ne::transform3f t;
		t.position.xy = { view.position.x, ground_y };
		t.scale.xy = { view.scale.width, 50.0f };
		ne::shader::set_transform(&t);
		ne::shader::set_color(1.0f, 1.0f, 1.0f, 0.1f);
		textures.blank.bind();
		still_quad().bind();
		still_quad().draw();

		// Object collisions.
		ne::shader::set_color(1.0f, 0.2f, 0.2f, 0.2f);
		for (auto& i : chunks) {
			i.second.objects.each<ne::game_object>([&](ne::game_object* object) {
				ne::shader::set_transform(&object->collision_transform());
				still_quad().draw();
			});
		}
	}
}

void game_world::write(ne::memory_buffer* buffer) {
	buffer->write_float(ground_y);
	buffer->write_int32(level_num);
}

void game_world::read(ne::memory_buffer* buffer) {
	ground_y = buffer->read_float();
	level_num = buffer->read_int32();
}

void game_world::change(int level_num) {
	change_to_level_num = level_num;
}

void game_world::after_load() {
	if (level_num == 0 && save_data) {
		// Open the doors for completed levels in the safehouse.
		save_data->each_completed_level([&](int key, level_complete_data complete_data) {
			each<door_object>([&](auto door) {
				if (key == door->leads_to_level_num) {
					door->is_open = true;
				}
			});
		});
	}
}
