#pragma once

#include <game_object.hpp>

#define OBJECT_TYPE_PLAYER     0
#define OBJECT_TYPE_CHASER     1
#define OBJECT_TYPE_DECORATION 2
#define TOTAL_OBJECT_TYPES     3

class game_object_definitions : public ne::game_object_definitions {
public:
	void initialize();
};
