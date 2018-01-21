#pragma once

enum class game_object_type {
	any = -1,
	player,
	chaser,
	total
};

class game_object;
class game_state;

game_object* create_object(game_state* game, game_object_type type);
