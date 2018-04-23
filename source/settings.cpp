#include "settings.hpp"
#include "assets.hpp"
#include "game.hpp"
#include "menu.hpp"

#include <platform.hpp>
#include <memory.hpp>

float settings::music_volume = 0.6f;
float settings::sound_volume = 0.6f;
int settings::difficulty = DIFFICULTY_EASY;
#if _DEBUG
bool settings::show_debug = true;
#else
bool settings::show_debug = false;
#endif

float settings::ref_music_volume = 1.0f;

void settings::save() {
	ne::memory_buffer buffer;
	buffer.write_float(music_volume);
	buffer.write_float(sound_volume);
	buffer.write_int32(difficulty);
	buffer.write_uint8(show_debug ? 1 : 0);
	ne::write_file("saves/settings.conf", buffer.begin, buffer.write_index());
	buffer.free();
}

void settings::load() {
	if (!ne::file_exists("saves/settings.conf")) {
		return;
	}
	ne::memory_buffer buffer;
	ne::read_file("saves/settings.conf", &buffer);
	music_volume = buffer.read_float();
	sound_volume = buffer.read_float();
	difficulty = buffer.read_int32();
	show_debug = (buffer.read_uint8() != 0);
	buffer.free();
}

void settings::play(ne::sound* sound, float volume, int loops) {
	sound->play((int)(sound_volume * volume * 100.0f), loops);
}

void settings::play(ne::music* music, float volume) {
	ref_music_volume = volume;
	ne::music::set_volume((int)(music_volume * volume * 100.0f));
	music->play();
}

void settings::update_volume() {
	ne::music::set_volume((int)(music_volume * ref_music_volume * 100.0f));
}

settings_state::settings_state() {
	difficulty.font = &fonts.hud;
	difficulty.render("Difficulty");
	difficulties.font = &fonts.hud_small;
	difficulties.button_downscale = { 16.0f, 4.0f };
	difficulties.is_horizontal = true;
	difficulties.add_button("Easy", [this] {
		settings::difficulty = DIFFICULTY_EASY;
		settings::save();
		difficulties.set_active(0);
	});
	difficulties.add_button("Normal", [this] {
		settings::difficulty = DIFFICULTY_NORMAL;
		settings::save();
		difficulties.set_active(1);
	});
	difficulties.add_button("Hard", [this] {
		settings::difficulty = DIFFICULTY_HARD;
		settings::save();
		difficulties.set_active(2);
	});
	switch (settings::difficulty) {
	case DIFFICULTY_EASY:
		difficulties.set_active(0);
		break;
	case DIFFICULTY_NORMAL:
		difficulties.set_active(1);
		break;
	case DIFFICULTY_HARD:
		difficulties.set_active(2);
		break;
	default:
		break;
	}
	music_volume.font = &fonts.hud;
	music.font = &fonts.hud;
	music.button_downscale = { 16.0f, 4.0f };
	music.is_horizontal = true;
	music.has_background = false;
	music.add_button("-", [] {
		settings::music_volume -= 0.1f;
		if (settings::music_volume < 0.0f) {
			settings::music_volume = 0.0f;
		}
		settings::update_volume();
		settings::save();
	});
	music.add_button("+", [] {
		settings::music_volume += 0.1f;
		if (settings::music_volume > 1.0f) {
			settings::music_volume = 1.0f;
		}
		settings::update_volume();
		settings::save();
	});

	sound_volume.font = &fonts.hud;
	sound.font = &fonts.hud;
	sound.button_downscale = { 16.0f, 4.0f };
	sound.is_horizontal = true;
	sound.has_background = false;
	sound.add_button("-", [] {
		settings::sound_volume -= 0.1f;
		if (settings::sound_volume < 0.0f) {
			settings::sound_volume = 0.0f;
		}
		settings::save();
	});
	sound.add_button("+", [] {
		settings::sound_volume += 0.1f;
		if (settings::sound_volume > 1.0f) {
			settings::sound_volume = 1.0f;
		}
		settings::save();
	});

	other.font = &fonts.hud_small;
	other.button_downscale = { 16.0f, 4.0f };
	other.has_background = false;
	other.add_button("Back", [] {
		ne::swap_state<menu_state>();
	});
}

settings_state::~settings_state() {
	
}

void settings_state::update() {
	camera.transform.scale.xy = ne::window_size().to<float>();
	camera.update();
	camera.bind();

	difficulty.transform.position.x = camera.width() / 2.0f - difficulty.transform.scale.width / 2.0f;
	difficulty.transform.position.y = 350.0f;
	difficulties.update(camera.size(), { -135.0f, 340.0f }, textures.bg.menu.size.to<float>());

	music_volume.render(STRING("Music (" << (int)(settings::music_volume * 100.1f) << "%)"));
	music_volume.transform.position.x = camera.width() / 2.0f - music_volume.transform.scale.width / 2.0f;
	music_volume.transform.position.y = 470.0f;
	music.update(camera.size(), { -60.0f, 460.0f }, textures.bg.menu.size.to<float>());

	sound_volume.render(STRING("Sound (" << (int)(settings::sound_volume * 100.1f) << "%)"));
	sound_volume.transform.position.x = camera.width() / 2.0f - sound_volume.transform.scale.width / 2.0f;
	sound_volume.transform.position.y = 590.0f;
	sound.update(camera.size(), { -60.0f, 580.0f }, textures.bg.menu.size.to<float>());

	other.update(camera.size(), { 0.0f, 410.0f }, textures.bg.menu.size.to<float>());
}

void settings_state::draw() {
	shaders.basic.bind();
	camera.bind();
	textures.bg.menu.bind();
	ne::transform3f bg;
	bg.scale.xy = textures.bg.menu.size.to<float>() * 2.0f;
	ne::shader::set_transform(&bg);
	ne::shader::set_color(1.0f);
	still_quad().draw();
	difficulties.draw(camera.size(), 0.0f, textures.ui.popup.size.to<float>() / 2.0f);
	music.draw(0.0f, 0.0f, 0.0f);
	sound.draw(0.0f, 0.0f, 0.0f);
	other.draw(0.0f, 0.0f, 0.0f);
	ne::shader::set_color(1.0f);
	difficulty.draw();
	music_volume.draw();
	sound_volume.draw();
}
