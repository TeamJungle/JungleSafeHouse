#pragma once

#include "game_world_background.hpp"
#include "game_object_types.hpp"
#include "npc_object.hpp"
#include "settings.hpp"

#include <game_world.hpp>
#include <game_object.hpp>
#include <timer.hpp>

class player_object;
class game_state;
class game_save_data;

class game_world;

class point_light {
public:

	float intensity = 0.998f;
	ne::vector3f color = { 1.0f, 0.9f, 0.8f };
	int object_id = -1;

	void bind(int index, game_world* world);

};

class game_world : public ne::game_world {
public:

	float ground_y = 600.0f;
	bool draw_collisions = false;
	game_save_data* save_data = nullptr;
	int level_num = -1;
	std::vector<point_light> lights;
	float base_light = 1.0f;

	struct world_backgrounds {
		// Background:
		game_world_background background;
		game_world_background trees;
		game_world_background fog_back;
		game_world_background mid;
		// Foreground:
		game_world_background top;
		game_world_background top_lines;
		game_world_background bottom;
		game_world_background fog_front;

		void set_default();
	} backgrounds;

	game_world();

	void update() override;
	void draw(const ne::transform3f& view) override;

	void write(ne::memory_buffer* buffer) override;
	void read(ne::memory_buffer* buffer) override;

	void change(int level_num);
	void after_load() override;

	shop_ui shop;

private:

	int change_to_level_num = -1;
	
};
