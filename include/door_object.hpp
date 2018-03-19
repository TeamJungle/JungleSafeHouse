#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>
#include <graphics.hpp>

#define DOOR_REGULAR 0
#define DOOR_YELLOW  1
#define DOOR_RED     2
#define TOTAL_DOORS  3

class door_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_DOOR, "Door", false);

	bool is_open = false;
	int leads_to_level_num = 0;
	int opening_cost = 0;

private:

	ne::font_text level_label;
	ne::font_text cost_label;
	ne::sprite_animation coin_animation;
	bool is_near_player = false;

};
