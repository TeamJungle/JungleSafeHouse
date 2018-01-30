#pragma once

#include <input.hpp>

class game_input {
public:

	ne::input_action_listener interact;
	ne::input_action_listener pause;

	ne::input_action_listener ui_up;
	ne::input_action_listener ui_left;
	ne::input_action_listener ui_down;
	ne::input_action_listener ui_right;

	ne::input_active_indicator up;
	ne::input_active_indicator left;
	ne::input_active_indicator down;
	ne::input_active_indicator right;

	ne::input_action_listener up_hit;
	ne::input_action_listener left_hit;
	ne::input_action_listener down_hit;
	ne::input_action_listener right_hit;

	game_input();

};

game_input& input();
