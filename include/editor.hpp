#pragma once

#include "world.hpp"
#include "button_menu.hpp"

#include <engine.hpp>

// We don't want to let the players edit the safe house
// and other things that might ruin the game experience.
#ifdef _DEBUG
#define DEVELOPMENT_EDITOR 1
#else
#define DEVELOPMENT_EDITOR 0
#endif

#define EDITOR_TOOL_SELECT 0
#define EDITOR_TOOL_PLACE  1
#define EDITOR_TOOL_REMOVE 2

// Change this number to the world you want to edit.
#define EDITOR_CURRENT_WORLD_NUM   0

#define EDITOR_CURRENT_WORLD STRING("worlds/local/" << EDITOR_CURRENT_WORLD_NUM << ".world")

class editor_state : public ne::program_state {
public:

	ne::ortho_camera camera;
	ne::ortho_camera ui_camera;
	game_world world;

	editor_state();
	~editor_state() override;

	void update() override;
	void draw() override;

private:
	
	void save();

	bool saved = true;
	int tool = EDITOR_TOOL_SELECT;
	ne::game_object* selected = nullptr;
	float object_rotation = 0.0f;
	float object_scale = 1.0f;
	ne::game_object_meta place_meta;
	ne::transform3f left_panel;
	bool any_context_open = false;

	int key_listener = -1;
	int click_listener = -1;
	int wheel_listener = -1;

	ne::debug_info debug;

};
