#include "game.hpp"
#include "assets.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"

#include <graphics.hpp>
#include <platform.hpp>

void game_save_data::write(ne::memory_buffer* buffer) {
	buffer->write_int32(coins);
	buffer->write_uint32(levels_completed.size());
	for (auto& i : levels_completed) {
		buffer->write_int32(i.second.num);
	}
}

void game_save_data::read(ne::memory_buffer* buffer) {
	coins = buffer->read_int32();
	uint32 levels_done = buffer->read_uint32();
	levels_completed.clear();
	for (uint32 i = 0; i < levels_done; i++) {
		int32 num = buffer->read_int32();
		levels_completed[num] = { num };
	}
}

bool game_save_data::is_level_completed(int num) const {
	return levels_completed.find(num) != levels_completed.end();
}

void game_save_data::complete_level(int num) {
	if (num == 0 || is_level_completed(num)) {
		return;
	}
	levels_completed[num] = { num };
	coins += 250;
	saved = false;
	must_save = true;
}

void game_save_data::each_completed_level(const std::function<void(int, level_complete_data)>& func) {
	for (auto& i : levels_completed) {
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

	ne::set_music_volume(10);
	//music.jungle.play();

	listener.pause = input().pause.listen([this] {
		pause.active = !pause.active;
	});

	autosave_timer.start();
}

game_state::~game_state() {
	ne::erase(&listener.pause);
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
	if (!pause.active) {
		world.update();
	}

	coins_label.font = &fonts.hud;
	coins_label.render(STRING(save_data.get_coins()));
	coins_label.transform.position.x = ui_camera.width() / 2.0f - coins_label.transform.scale.width / 2.0f - textures.coin.size.to<float>().x / 2.0f;
	coins_label.transform.position.y = 32.0f;

	debug.set(&fonts.debug, STRING(
		"Delta " << ne::delta() << 
		"\nFPS: " << ne::current_fps()
	));

	if (autosave_timer.seconds() > 30 || save_data.must_be_saved()) {
		save();
		autosave_timer.start();
	}
}

void game_state::draw() {
	shaders.basic.bind();
	ne::transform3f view;
	
	// World
	ne::shader::set_color(1.0f);
	camera.bind();
	view.position.xy = camera.xy();
	view.scale.xy = camera.size();
	world.draw(view);

	// UI
	ui_camera.bind();
	view.position.xy = ui_camera.xy();
	view.scale.xy = ui_camera.size();
	coins_label.draw();
	// TODO: Clean
	{
		textures.coin.bind();
		ne::transform3f t;
		t.position = coins_label.transform.position;
		t.scale.xy = textures.coin.size.to<float>();
		t.position.x -= t.scale.width;
		ne::shader::set_transform(&t);
		still_quad().bind();
		still_quad().draw();
	}
	pause.draw(ui_camera.size());
	ne::shader::set_color(1.0f);
	still_quad().bind();
	debug.draw(view);
}

void game_state::save() {
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
