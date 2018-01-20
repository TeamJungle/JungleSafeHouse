#include "game_object_types.hpp"
#include "player_object.hpp"

game_object* create_object(game_state* game, game_object_type type) {
	switch (type) {
	case game_object_type::player: return new player_object(game);
	default: return nullptr;
	}
}
