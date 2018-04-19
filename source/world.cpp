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

void rain_particles::start() {
	raining = true;
	settings::play(&audio.rain, 0.3f, -1);
}

void rain_particles::stop() {
	raining = false;
	particles.clear();
	audio.rain.stop();
}

int rain_particles::count() const {
	return (int)particles.size();
}

void rain_particles::update(game_world* world) {
	if (raining && ne::current_frame() % 10 == 0) {
		float x = ne::ortho_camera::bound()->x();
		for (int i = 0; i < 40; i++) {
			particles.push_back({ x + (float)i * 32.0f - 16.0f, 16.0f });
			particles.push_back({ x + (float)i * 32.0f, 64.0f });
			particles.push_back({ x + (float)i * 32.0f + 16.0f, 32.0f });
		}
	}
	for (size_t i = 0; i < particles.size(); i++) {
		particles[i].x -= 6.0f;
		particles[i].y += 6.0f + ne::random_float(6.0f);
		if (particles[i].y > 700.0f) {
			particles.erase(particles.begin() + i);
			i--;
		}
	}
}

void rain_particles::draw() {
	ne::shader::set_color(1.0f, 1.0f, 1.0f, 0.75f);
	textures.rain.bind();
	still_quad().bind();
	ne::transform3f transform;
	transform.scale.xy = textures.rain.size.to<float>();
	for (auto& i : particles) {
		transform.position.xy = i;
		ne::shader::set_transform(&transform);
		still_quad().draw();
	}
	ne::shader::set_color(1.0f);
}

bool rain_particles::is_raining() const {
	return raining;
}

void thunder_effect::strike(const ne::vector2f& position) {
	last_strike.start();
	thunder_played = false;
	transform.position.xy = position;
	transform.scale.xy = textures.lightning.size.to<float>() / 1.8f;
	transform.scale.xy.ceil();
}

void thunder_effect::update(game_world* world) {
	if (!last_strike.has_started || last_strike.milliseconds() < 250) {
		return;
	}
	if (!thunder_played) {
		thunder_played = true;
		int sound = ne::random_int(0, 2);
		settings::play(&audio.thunder[sound], 0.5f);
	}
}

void thunder_effect::draw() {
	if (!last_strike.has_started || last_strike.milliseconds() > 1250) {
		return;
	}
	textures.lightning.bind();
	ne::shader::set_transform(&transform);
	still_quad().bind();
	still_quad().draw();
}

bool point_light::bind(int index, game_world* world) {
	if (index < 0 || index >= 40) {
		return false;
	}
	auto object = world->find_object<ne::game_object>(object_id);
	if (!object) {
		return false;
	}
	ne::ortho_camera* camera = ne::ortho_camera::bound();

	ne::vector2f position = object->transform.position.xy + object->transform.scale.xy / 2.0f;
	position.x += std::cos(rotate_current) * rotate_distance;
	position.y += std::sin(rotate_current) * rotate_distance;
	rotate_current += rotate_speed;

	if (camera->x() > position.x + 150.0f) {
		return false;
	}
	if (camera->x() + camera->width() < position.x - 150.0f) {
		return false;
	}

	std::string index_str = std::to_string(index);
	glm::mat4 view = ortho_view(camera->transform.position, camera->transform.rotation.z, camera->zoom);
	glm::vec4 view_position = view * glm::vec4(position.x, position.y, 1.0f, 1.0f);
	int light_position_handle = ne::shader::get_variable_handle("uni_LightPosition[" + index_str + "]");
	int light_color_handle = ne::shader::get_variable_handle("uni_LightColor[" + index_str + "]");
	int light_intensity_handle = ne::shader::get_variable_handle("uni_LightIntensity[" + index_str + "]");
	ne::shader::set_variable(light_position_handle, ne::vector2f{ view_position.x, view_position.y });
	ne::shader::set_variable(light_color_handle, color);
	ne::shader::set_variable(light_intensity_handle, intensity);
	return true;
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
	fog_front.speed = 4500.0f;
	fog_front.x_vary = 128.0f;
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

	if (rain_triggers.size() > 0) {
		auto trigger = rain_triggers.front();
		auto player = first<player_object>();
		if (player && trigger.first < player->transform.position.x) {
			if (trigger.second) {
				rain.start();
			} else {
				rain.stop();
			}
			rain_triggers.erase(rain_triggers.begin());
		}
	}

	// Handle updates which may destroy objects.
	each_if<player_object>([&](auto player) {
		if (rain.is_raining() && ne::current_frame() % 120 == 0) {
			float dir = (ne::random_chance(0.5f) ? 1.0f : -1.0f);
			thunder.strike({ player->transform.position.x + ne::random_float(400.0f, 600.0f) * dir, 100.0f });
		}
		// Collect an item if colliding.
		if (save_data) {
			each_if<item_object>([&](auto item) {
				if (!item->collision_transform().collides_with(player->collision_transform())) {
					return true;
				}
				if (item->subtype == ITEM_GEM) {
					save_data->add_gems(1);
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

	rain.update(this);
	thunder.update(this);

	// Change world.
	if (change_to_level_num >= 0) {
#if _DEBUG
		std::string path = STRING("worlds/local/" << change_to_level_num << ".world");
#else
		std::string path = STRING("worlds/" << change_to_level_num << ".world");
#endif
		if (ne::file_exists(path)) {
			load(path);
			level_num = change_to_level_num; // just in case of old level num
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
		bound_lights = 0;
		for (size_t i = 0; i < lights.size(); i++) {
			if (!find_object<ne::game_object>(lights[i].object_id)) {
				lights.erase(lights.begin() + i);
				i--;
				continue;
			}
			if (lights[i].bind(bound_lights, this)) {
				bound_lights++;
			}
		}
		ne::shader::set_variable(ne::shader::get_variable_handle("uni_LightCount"), bound_lights);
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

	// Draw effects.
	rain.draw();
	thunder.draw();

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
	buffer->write_int32(-5);
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
	// version 5
	buffer->write_uint32(rain_triggers.size());
	for (auto& i : rain_triggers) {
		buffer->write_float(i.first);
		buffer->write_uint8((uint8)i.second);
	}
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
	if (version > 4) {
		uint32 trigger_count = buffer->read_uint32();
		NE_INFO(trigger_count);
		for (uint32 i = 0; i < trigger_count; i++) {
			float x = buffer->read_float();
			uint8 type = (buffer->read_uint8() != 0);
			rain_triggers.push_back({ x, type });
		}
	}
}

void game_world::change(int to_level_num) {
	if (level_num == 0 && to_level_num == 0) {
		auto player = first<player_object>();
		if (player) {
			player->transform.position.x = 3600.0f;
		}
		return;
	}
	change_to_level_num = to_level_num;
}

void game_world::after_load() {
	if (level_num == 0 && save_data) {
		// Open the doors for completed levels in the safehouse.
		save_data->each_completed_level([&](int key, level_complete_data complete_data) {
			each<door_object>([&](auto door) {
				if (key == door->leads_to_level_num && complete_data.state != LEVEL_IS_LOCKED) {
					door->is_open = true;
				}
			});
		});
		// Add doors for the user levels.
		float x = 3700.0f;
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
	// Ensure we don't have objects that have floating pixels.
	each<ne::game_object>([](auto object) {
		object->transform.position.xy.ceil();
	});
}

void game_world::reset() {
	ne::game_world::reset();
	ground_y = 600.0f;
	level_num = -1;
	base_light = 1.0f;
	lights.clear();
}
