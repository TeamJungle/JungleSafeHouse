#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>

#define DECORATION_TREE                   0
#define DECORATION_LOG_1                  1
#define DECORATION_LOG_2                  2
#define DECORATION_FLOWER_1               3
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
#define DECORATION_FLOWER_2               18
#define TOTAL_DECORATIONS                 19

class decoration_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_DECORATION, "Decoration", false);

};
