#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>

#define ITEM_COIN       0
#define ITEM_CHERRY     1
#define ITEM_MANDARIN   2
#define ITEM_PEAR       3
#define ITEM_STRAWBERRY 4
#define ITEM_GEM        5
#define ITEM_BOOTS      6
#define ITEM_MACHETE    7
#define TOTAL_ITEMS     8

class item_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_ITEM, "Item", false);

};

