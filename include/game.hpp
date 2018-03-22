#pragma once

#include "game_input.hpp"
#include "world.hpp"
#include "pause.hpp"

#include <engine.hpp>
#include <camera.hpp>
#include <ui.hpp>

class player_object;

struct level_complete_data {
	int num = 0;
};

class game_save_data {
public:

	void write(ne::memory_buffer* buffer);
	void read(ne::memory_buffer* buffer);

	bool is_level_completed(int num) const;
	void complete_level(int num);
	void each_completed_level(const std::function<void(int, level_complete_data)>& func);
	bool is_saved() const;
	bool must_be_saved() const;
	void mark_saved();

	void add_coins(int amount);
	int get_coins() const;

	void add_gem(int amount);
	int get_gem() const;

	void give_machete();
	bool has_machete() const;

private:

	bool saved = true;
	bool must_save = false;
	int32 coins = 0;
	int32 gems = 0;
	std::unordered_map<int, level_complete_data> levels_completed;
	bool item_machete = false;

};

class game_state : public ne::program_state {
public:

	game_save_data save_data;
	ne::font_text coins_label;
	ne::font_text gems_label;

	pause_menu pause;

	ne::ortho_camera camera;
	ne::ortho_camera ui_camera;
	game_world world;

	game_state();
	~game_state() override;

	void update() override;
	void draw() override;

	void save();
	void load();

private:

	ne::debug_info debug;
	ne::timer autosave_timer;
	
	struct {
		int pause = -1;
	} listener;

};

ne::drawing_shape& still_quad();
ne::drawing_shape& animated_quad();
