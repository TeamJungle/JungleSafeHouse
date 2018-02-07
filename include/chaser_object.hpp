#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>

#define CHASER_TIGER  0
#define TOTAL_CHASERS 1

class chaser_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_CHASER, "Chaser", false);

	chaser_object();

};
