#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>
#include <graphics.hpp>

#define VINE_1       0
#define VINE_2       1
#define VINE_FLOWER  2
#define TOTAL_VINES  3

class vine_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_VINE, "Vine", false);

	bool flip_x = false;
	float health = 1.0f;

};
