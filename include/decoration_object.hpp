#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>

#define DECORATION_TREE                   0
#define DECORATION_LOG_1                  1
#define DECORATION_FLOWER_1               2
#define DECORATION_FLOWER_2               3
#define DECORATION_TENTACLE_1             4
#define DECORATION_MUSHROOM_1             5
#define DECORATION_MUSHROOM_2             6
#define DECORATION_MUSHROOM_3             7
#define DECORATION_GROUND_MIXED           8
#define DECORATION_GROUND_GRASS           9
#define DECORATION_GROUND_STONE           10
#define DECORATION_GROUND_WATER           11
#define DECORATION_GROUND_BIG_STONE_1     12
#define DECORATION_GROUND_BIG_STONE_2     13
#define DECORATION_GROUND_BIG_STONE_3     14
#define DECORATION_GROUND_BIG_STONE_4     15
#define DECORATION_GROUND_BIG_STONE_5     16
#define DECORATION_GROUND_BIG_STONE_6     17
#define DECORATION_PLATFORM_TOP_1         18
#define DECORATION_PLATFORM_TOP_2         19
#define DECORATION_PLATFORM_BOTTOM        20
#define DECORATION_PLATFORM_LOG           21
#define DECORATION_PLATFORM_BOARD_1       22
#define DECORATION_PLATFORM_BOARD_2       23
#define DECORATION_PLATFORM_BOARD_3       24
#define DECORATION_LOG_HOLLOW_FRONT       25
#define DECORATION_LOG_HOLLOW_BACK        26
#define DECORATION_SAFEHOUSE_WALL         27
#define TOTAL_DECORATIONS                 28

class decoration_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_DECORATION, "Decoration", false);

	bool flip_x = false;
	bool flip_y = false;

};
