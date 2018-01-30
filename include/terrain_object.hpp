#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>

class terrain_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_TERRAIN, "Terrain", false);

};