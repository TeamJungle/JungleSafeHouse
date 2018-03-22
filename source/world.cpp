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

void point_light::bind(int index, game_world* world) {
	if (index < 0 || index >= 20) {
		return;
	}
	auto object = world->find_object<ne::game_object>(object_id);
	if (!object) {
		return;
	}
	ne::vector2f position = object->transform.position.xy + object->transform.scale.xy / 2.0f;
	position.x += std::cos(rotate_current) * rotate_distance;
	position.y += std::sin(rotate_current) * rotate_distance;
	rotate_current += rotate_speed;

	std::string index_str = std::to_string(index);
	ne::ortho_camera* camera = ne::ortho_camera::bound();
	glm::mat4 view = ortho_view(camera->transform.position, camera->transform.rotation.z, camera->zoom);
	glm::vec4 view_position = view * glm::vec4(position.x, position.y, 1.0f, 1.0f);
	int light_position_handle = ne::shader::get_variable_handle("uni_LightPosition[" + index_str + "]");
	int light_color_handle = ne::shader::get_variable_handle("uni_LightColor[" + index_str + "]");
	int light_intensity_handle = ne::shader::get_variable_handle("uni_LightIntensity[" + index_str + "]");
	ne::shader::set_variable(light_position_handle, ne::vector2f{ view_position.x, view_position.y });
	ne::shader::set_variable(light_color_handle, color);
	ne::shader::set_variable(light_intensity_handle, intensity);
}

void game_world::world_backgrounds::set_default() {
	// Set names.
	background.name = "Background";
	trees.name = "Trees";
	fog_back.name = "Fog (back)";
	mid.name = "Middle";
	top.name = "Top";
	top_lines.name = "Top lines";
	bottom.name = "Bottom";
	fog_front.name = "Fog (front)";

	// Configure backgrounds
	background.top_offset.y = -300.0f;
	background.zoom = 0.5f;
	trees.top_offset.y = 250.0f;
	trees.zoom = 0.5f;
	mid.top_offset.y = 200.0f;
	mid.zoom = 0.5f;

	// Configure foregrounds
	bottom.zoom = 1.0f;
	bottom.zoom = 0.4f;
	bottom.top_offset.y = 400.0f;
	bottom.top_offset.y = 300.0f;
	bottom.bottom_offset.y = 322.0f * 0.75f;
	bottom.bottom_offset.y = 192.0f * 0.75f;

	fog_back.top_offset.y = 250.0f;
	fog_back.speed = 500.0f;
	fog_back.x_vary = 16.0f;
	fog_back.timer.start();

	fog_front.zoom = 0.5f;
	fog_front.top_offset.x = 100.0f;
	fog_front.top_offset.y = 400.0f;
	fog_front.speed = 1500.0f;
	fog_front.x_vary = 16.0f;
	fog_front.timer.start();

	top.zoom = 0.75f;
	top.top_offset.y = 150.0f;

	top_lines.zoom = 0.5f;
	top_lines.top_offset.y = 150.0f;
}

void game_world::world_backgrounds::write(ne::memory_buffer* buffer) {
	buffer->write_uint32(8); // How many backgrounds - might be useful in future
	background.write(buffer);
	trees.write(buffer);
	fog_back.write(buffer);
	mid.write(buffer);
	top.write(buffer);
	top_lines.write(buffer);
	bottom.write(buffer);
	fog_front.write(buffer);
}

void game_world::world_backgrounds::read(ne::memory_buffer* buffer) {
	uint32 count = buffer->read_uint32();
	background.read(buffer);
	trees.read(buffer);
	fog_back.read(buffer);
	mid.read(buffer);
	top.read(buffer);
	top_lines.read(buffer);
	bottom.read(buffer);
	fog_front.read(buffer);
}

game_world::game_world() {
	definitions.objects.meta = new game_object_definitions();
	definitions.objects.meta->initialize();
	shop.world = this;
	init();
}

void game_world::init() {
	place_chunk(0, 0);
	backgrounds = {};
	backgrounds.set_default();
}

void game_world::update() {
	update_objects();
	shop.update();

	// Handle updates which may destroy objects.
	each_if<player_object>([&](auto player) {
		// Collect an item if colliding.
		if (save_data) {
			each_if<item_object>([&](auto item) {
				if (!item->collision_transform().collides_with(player->collision_transform())) {
					return true;
				}
				if (item->subtype == ITEM_GEM) {
					save_data->add_gem(1);
				} else {
					save_data->add_coins(5);
				}
				
				// TODO: Clean.
				{
					static int last_pickup_sound = 0;
					settings::play(&audio.pickup[last_pickup_sound], 0.02f);
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
		bool alive = each_if<chaser_object>([&](auto chaser) {
			if (player->collision_transform().collides_with(chaser->collision_transform())) {
				destroy_object(player->id, nullptr);
				settings::play(&audio.tiger, 0.15f);
				change(0);
				return false;
			}
			return true;
		});
		if (!alive) {
			return false;
		}
		// Die if colliding with spikes.
		alive = each_if<spikes_object>([&](auto spikes) {
			if (player->collision_transform().collides_with(spikes->collision_transform())) {
				destroy_object(player->id, nullptr);
				change(0);
				return false;
			}
			return true;
		});
		return alive;
	});
	each_if<vine_object>([this](auto vine) {
		if (vine->health < 0.01f) {
			destroy_object(vine->id, nullptr);
			return false;
		}
		return true;
	});

	// Change world.
	if (change_to_level_num >= 0) {
#if _DEBUG
		std::string path = STRING("worlds/local/" << change_to_level_num << ".world");
#else
		std::string path = STRING("worlds/" << change_to_level_num << ".world");
#endif
		if (ne::file_exists(path)) {
			load(path);
		} else {
			ne::show_message("world \"" + path + "\" does not exist.");
		}
		change_to_level_num = -1;
	}
}

void game_world::draw(const ne::transform3f& view) {
	// Lights.
	if (lights.size() > 0 || base_light < 0.99f) {
		shaders.light.bind();
		ne::shader::set_color(1.0f);
		ne::ortho_camera::bound()->bind();
		for (size_t i = 0; i < lights.size(); i++) {
			if (!find_object<ne::game_object>(lights[i].object_id)) {
				lights.erase(lights.begin() + i);
				i--;
				continue;
			}
			lights[i].bind(i, this);
		}
		ne::shader::set_variable(ne::shader::get_variable_handle("uni_LightCount"), (int)lights.size());
		ne::shader::set_variable(ne::shader::get_variable_handle("uni_BaseLight"), base_light);
	} else {
		shaders.basic.bind();
	}

	// Backgrounds.
	backgrounds.background.draw(view, &textures.bg.high_bright);
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

	// Shop.
	shaders.basic.bind();
	shop.draw(view);

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
	buffer->write_int32(-4);
	buffer->write_int32(level_num);
	buffer->write_uint8(backgrounds.background.is_visible ? 1 : 0);
	buffer->write_uint8(backgrounds.trees.is_visible ? 1 : 0);
	buffer->write_uint8(backgrounds.fog_back.is_visible ? 1 : 0);
	buffer->write_uint8(backgrounds.mid.is_visible ? 1 : 0);
	buffer->write_uint8(backgrounds.top.is_visible ? 1 : 0);
	buffer->write_uint8(backgrounds.top_lines.is_visible ? 1 : 0);
	buffer->write_uint8(backgrounds.bottom.is_visible ? 1 : 0);
	buffer->write_uint8(backgrounds.fog_front.is_visible ? 1 : 0);
	buffer->write_uint32(lights.size());
	for (auto& i : lights) {
		buffer->write_float(i.intensity);
		buffer->write_vector3f(i.color);
		buffer->write_int32(i.object_id);
		buffer->write_float(i.rotate_speed);
		buffer->write_float(i.rotate_distance);
	}
	buffer->write_float(base_light);
	backgrounds.write(buffer);
}

void game_world::read(ne::memory_buffer* buffer) {
	ground_y = buffer->read_float();
	int version = buffer->read_int32();
	if (version < 0) {
		level_num = buffer->read_int32();
	} else {
		// This is an old world format.
		level_num = version;
		return;
	}
	version = std::abs(version);
	backgrounds.background.is_visible = (buffer->read_uint8() != 0);
	backgrounds.trees.is_visible = (buffer->read_uint8() != 0);
	backgrounds.fog_back.is_visible = (buffer->read_uint8() != 0);
	backgrounds.mid.is_visible = (buffer->read_uint8() != 0);
	backgrounds.top.is_visible = (buffer->read_uint8() != 0);
	backgrounds.top_lines.is_visible = (buffer->read_uint8() != 0);
	backgrounds.bottom.is_visible = (buffer->read_uint8() != 0);
	backgrounds.fog_front.is_visible = (buffer->read_uint8() != 0);
	if (version > 1) {
		uint32 light_count = buffer->read_uint32();
		for (uint32 i = 0; i < light_count; i++) {
			float intensity = buffer->read_float();
			ne::vector3f color = buffer->read_vector3f();
			int32 object_id = buffer->read_int32();
			float rotate_speed = 0.0f;
			float rotate_distance = 0.0f;
			if (version > 3) {
				rotate_speed = buffer->read_float();
				rotate_distance = buffer->read_float();
			}
			lights.push_back({ intensity, color, object_id, 0.0f, rotate_speed, rotate_distance });
		}
		base_light = buffer->read_float();
	}
	if (version > 2) {
		backgrounds = {};
		backgrounds.read(buffer);
	}
}

void game_world::change(int to_level_num) {
	if (level_num == 0 && to_level_num == 0) {
		first<player_object>()->transform.position.x = 3300.0f;
		return;
	}
	change_to_level_num = to_level_num;
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
		// Add doors for the user levels.
		float x = 3400.0f;
		for (int i = 100; i < 1000; i++) {
#if _DEBUG
			std::string path = STRING("worlds/local/" << i << ".world");
#else
			std::string path = STRING("worlds/" << i << ".world");
#endif
			if (!ne::file_exists(path)) {
				continue;
			}
			auto meta = definitions.objects.meta->get_meta(OBJECT_TYPE_DOOR, DOOR_REGULAR);
			auto door = (door_object*)spawn(meta, { x, 520.0f });
			door->leads_to_level_num = i;
			door->transform.scale.x /= 2.0f;
			door->transform.scale.y /= 2.0f;
			x += 160.0f;
		}
	}
}

void game_world::reset() {
	ne::game_world::reset();
	ground_y = 600.0f;
	level_num = -1;
	base_light = 1.0f;
	lights.clear();
}
