#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>

#define SPIKES_1     0
#define SPIKES_2     1
#define TOTAL_SPIKES 2

class spikes_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_SPIKES, "Spikes", false);

	spikes_object();

};
