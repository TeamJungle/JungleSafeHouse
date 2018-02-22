#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>

class player_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_PLAYER, "Player", false);

	player_object();
	~player_object() override;

private:

	int up_hit = -1;
	int down_hit = -1;
	int state = 0;

};
