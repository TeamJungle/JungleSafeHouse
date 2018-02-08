#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>

#define SLIDE_UNDER_TREE 0

class slide_under_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_SLIDE_UNDER, "Slide under", false);

};
