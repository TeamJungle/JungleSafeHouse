#include "game.hpp"
#include "assets.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"

#include <graphics.hpp>
#include <platform.hpp>

void game_save_data::write(ne::memory_buffer* buffer) {
	buffer->write_int32(coins);
	buffer->write_int32(gems);
	buffer->write_uint8(item_boots ? 1 : 0);
	buffer->write_uint8(item_machete ? 1 : 0);
	buffer->write_uint32(levels.size());
	for (auto& i : levels) {
		buffer->write_int32(i.second.num);
		buffer->write_int32(i.second.state);
	}
}

void game_save_data::read(ne::memory_buffer* buffer) {
	coins = buffer->read_int32();
	gems = buffer->read_int32();
	item_boots = (buffer->read_uint8() != 0);
	item_machete = (buffer->read_uint8() != 0);
	uint32 levels_done = buffer->read_uint32();
	levels.clear();
	for (uint32 i = 0; i < levels_done; i++) {
		int32 num = buffer->read_int32();
		int32 state = buffer->read_int32();
		levels[num] = { num, state };
	}
}

bool game_save_data::is_level_completed(int num) const {
	return levels.find(num) != levels.end() && levels.find(num)->second.state == LEVEL_IS_COMPLETED;
}

void game_save_data::complete_level(int num) {
	if (num == 0 || is_level_completed(num)) {
		return;
	}
	levels[num] = { num, LEVEL_IS_COMPLETED };
	coins += 250;
	saved = false;
	must_save = true;
}

void game_save_data::unlock_level(int num) {
	if (num == 0 || is_level_completed(num)) {
		return;
	}
	levels[num] = { num, LEVEL_IS_UNLOCKED };
	saved = false;
	must_save = true;
}

void game_save_data::each_completed_level(const std::function<void(int, level_complete_data)>& func) {
	for (auto& i : levels) {
		func(i.first, i.second);
	}
}

bool game_save_data::is_saved() const {
	return saved;
}

bool game_save_data::must_be_saved() const {
	return must_save;
}

void game_save_data::mark_saved() {
	saved = true;
	must_save = false;
}

void game_save_data::add_coins(int amount) {
	coins += amount;
	saved = false;
}

int game_save_data::get_coins() const {
	return coins;
}

void game_save_data::add_gems(int amount) {
	gems += amount;
	saved = false;
}

int game_save_data::get_gems() const {
	return gems;
}

void game_save_data::give_machete() {
	item_machete = true;
	saved = false;
}

bool game_save_data::has_machete() const {
	return item_machete;
}

void game_save_data::give_boots() {
	item_boots = true;
	saved = false;
}

bool game_save_data::has_boots() const {
	return item_boots;
}

game_state::game_state() {
	camera.target_chase_aspect.y = 1.75f;
	camera.target_chase_speed = { 0.15f, 0.025f };
	camera.zoom = 2.0f;

	// Initialize world.
	load();
	world.save_data = &save_data;
#if _DEBUG
	world.load("worlds/local/0.world");
#else
	world.load("worlds/0.world");
#endif

	settings::play(&audio.jungle, 0.1f);

	listener.pause = input().pause.listen([this] {
		if (world.shop.is_open) {
			world.shop.owner->close_shop(&world.shop);
			return;
		}
		if (pause.is_open()) {
			pause.close();
		} else {
			pause.open();
		}
	});

	autosave_timer.start();
}

game_state::~game_state() {
	input().pause.erase(&listener.pause);
}

void game_state::update() {
	camera.transform.scale.xy = ne::window_size().to<float>();
	ui_camera.transform.scale.xy = ne::window_size().to<float>();

	auto player = world.first<player_object>();
	if (player) {
		camera.target = &player->transform;
	} else {
		camera.target = nullptr;
	}
	camera.update();

	pause.update(ui_camera.size());
	if (!pause.is_open()) {
		world.update();
	}

	coins_label.font = &fonts.hud;
	coins_label.render(STRING(save_data.get_coins()));
	coins_label.transform.position.x = ui_camera.width() / 2.0f - coins_label.transform.scale.width / 2.0f - textures.ui.coin.size.to<float>().x / 2.0f;
	coins_label.transform.position.y = 32.0f;

	gems_label.font = &fonts.hud;
	gems_label.render(STRING(save_data.get_gems()));
	gems_label.transform.position.x = ui_camera.width() / 2.0f - gems_label.transform.scale.width / 2.0f - textures.ui.gem.size.to<float>().x / 2.0f;
	gems_label.transform.position.y = 96.0f;

#if _DEBUG
	debug.set(&fonts.debug, STRING(
		"Delta " << ne::delta() <<
		"\nFPS: " << ne::current_fps() <<
		"\nRain particles: " << world.rain.count() <<
		"\nLights: " << world.bound_lights
	));
#endif

	if (autosave_timer.seconds() > 30 || save_data.must_be_saved()) {
		save();
		autosave_timer.start();
	}
}

void game_state::draw() {
	ne::transform3f view;
	
	// World
	ne::shader::set_color(1.0f);
	camera.bind();
	view.position.xy = camera.xy();
	view.scale.xy = camera.size();
	world.draw(view);

	// UI
	shaders.basic.bind();
	ui_camera.bind();
	view.position.xy = ui_camera.xy();
	view.scale.xy = ui_camera.size();
	coins_label.draw();
	gems_label.draw();
	// TODO: Clean
	{
		still_quad().bind();
		textures.ui.coin.bind();
		ne::transform3f t;
		t.scale.xy = textures.ui.coin.size.to<float>();
		t.position = coins_label.transform.position;
		t.position.x -= t.scale.width;
		ne::shader::set_transform(&t);
		still_quad().draw();
		textures.ui.gem.bind();
		t.scale.xy = textures.ui.gem.size.to<float>();
		t.position = gems_label.transform.position;
		t.position.x -= t.scale.width;
		ne::shader::set_transform(&t);
		still_quad().draw();
	}
	pause.draw(ui_camera.size());
	ne::shader::set_color(1.0f);
	still_quad().bind();
#if _DEBUG
	debug.draw(view);
#endif

	// Draw tutorial instructions:
	if (world.level_num == 1) {
		auto player = world.first<player_object>();
		if (player) {
			info_label.font = &fonts.hud;
			if (player->transform.position.x > 3800.0f) {
				info_label.render("Press 'Space' twice to open the door!");
			} else if (player->transform.position.x > 3100.0f) { // 'space' to chop vines
				info_label.render("Press 'Space' to chop vines!");
			} else if (player->transform.position.x > 2400.0f) { // 'w' mid-air to double jump
				info_label.render("        Grab the winged boots!\nPress 'W' mid-air to double jump!");
			} else if (player->transform.position.x > 1500.0f) { // 's' to slide
				info_label.render("Press 'S' to slide!");
			} else if (player->transform.position.x > 900.0f) { // 'w' to jump
				info_label.render("Press 'W' to jump!");
			}
			info_label.transform.position.x = ui_camera.width() / 2.0f - info_label.transform.scale.width / 2.0f;
			info_label.transform.position.y = 256.0f + std::sin((float)ne::ticks() / 100000.0f) * 10.0f;
			info_label.draw();
		}
	}

	// TODO: NPCs need this to be bound in update. 
	// This should be fixed by having a reference in world or something.
	camera.bind();
}

void game_state::save() {
	if (!std::experimental::filesystem::is_directory("saves")) {
		std::experimental::filesystem::create_directory("saves");
	}
	ne::memory_buffer buffer;
	save_data.write(&buffer);
	ne::write_file("saves/default.save", buffer.begin, buffer.write_index());
	save_data.mark_saved();
}

void game_state::load() {
	if (!ne::file_exists("saves/default.save")) {
		return;
	}
	ne::memory_buffer buffer;
	ne::read_file("saves/default.save", &buffer);
	save_data.read(&buffer);
	buffer.free();
}

ne::drawing_shape& still_quad() {
	static ne::drawing_shape shape;
	return shape;
}

ne::drawing_shape& animated_quad() {
	static ne::drawing_shape shape;
	return shape;
}
