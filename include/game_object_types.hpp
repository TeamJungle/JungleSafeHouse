#pragma once

#include <game_object.hpp>

#define OBJECT_TYPE_PLAYER      0
#define OBJECT_TYPE_CHASER      1
#define OBJECT_TYPE_DECORATION  2
#define OBJECT_TYPE_ITEM        3
#define OBJECT_TYPE_PLATFORM    4
#define OBJECT_TYPE_DOOR        5
#define TOTAL_OBJECT_TYPES      6

class game_object_definitions : public ne::game_object_definitions {
public:
	void initialize();
};
