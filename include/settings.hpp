#pragma once

#include "button_menu.hpp"

#include <engine.hpp>
#include <audio.hpp>

#define DIFFICULTY_EASY   0
#define DIFFICULTY_NORMAL 1
#define DIFFICULTY_HARD   2

class settings {
public:

	static float music_volume;
	static float sound_volume;
	static int difficulty;
	static bool show_debug;

	static void save();
	static void load();

	static void play(ne::sound* sound, float volume, int loops = 0);
	static void play(ne::music* music, float volume);
	static void update_volume();

private:

	// Used for update_volume() to set correct volume.
	static float ref_music_volume;

};

class settings_state : public ne::program_state {
public:

	ne::ortho_camera camera;

	settings_state();
	~settings_state() override;

	void update() override;
	void draw() override;

private:

	button_menu difficulties;
	ne::font_text difficulty;

	button_menu music;
	ne::font_text music_volume;

	button_menu sound;
	ne::font_text sound_volume;

	button_menu other;

};
