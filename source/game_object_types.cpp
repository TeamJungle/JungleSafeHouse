#include "game_object_types.hpp"
#include "player_object.hpp"
#include "chaser_object.hpp"
#include "decoration_object.hpp"
#include "item_object.hpp"
#include "platform_object.hpp"
#include "door_object.hpp"

void game_object_definitions::initialize() {
	meta.insert(meta.begin(), TOTAL_OBJECT_TYPES, {});

	// Players
	define<player_object>(0, "Player", 0);

	// Chasers
	define<chaser_object>(CHASER_TIGER, "Tiger", 0);

	// Decorations
	define<decoration_object>(DECORATION_TREE, "Tree", 0);
	define<decoration_object>(DECORATION_LOG_1, "Log", 0);
	define<decoration_object>(DECORATION_FLOWER_1, "Flowers (1)", 0);
	define<decoration_object>(DECORATION_FLOWER_2, "Flowers (2)", 0);
	define<decoration_object>(DECORATION_TENTACLE_1, "Tentacle", 0);
	define<decoration_object>(DECORATION_MUSHROOM_1, "Mushroom (1)", 0);
	define<decoration_object>(DECORATION_MUSHROOM_2, "Mushroom (2)", 0);
	define<decoration_object>(DECORATION_MUSHROOM_3, "Mushroom (3)", 0);
	define<decoration_object>(DECORATION_GROUND_MIXED, "Mixed ground", 0);
	define<decoration_object>(DECORATION_GROUND_GRASS, "Grass ground", 0);
	define<decoration_object>(DECORATION_GROUND_STONE, "Stone ground", 0);
	define<decoration_object>(DECORATION_GROUND_WATER, "Water ground", 0);
	define<decoration_object>(DECORATION_GROUND_BIG_STONE_1, "Big stone (1)", 0);
	define<decoration_object>(DECORATION_GROUND_BIG_STONE_2, "Big stone (2)", 0);
	define<decoration_object>(DECORATION_GROUND_BIG_STONE_3, "Big stone (3)", 0);
	define<decoration_object>(DECORATION_GROUND_BIG_STONE_4, "Big stone (4)", 0);
	define<decoration_object>(DECORATION_GROUND_BIG_STONE_5, "Big stone (5)", 0);
	define<decoration_object>(DECORATION_GROUND_BIG_STONE_6, "Big stone (6)", 0);
	define<decoration_object>(DECORATION_PLATFORM_TOP_1, "Platform top (1)", 0);
	define<decoration_object>(DECORATION_PLATFORM_TOP_2, "Platform top (2)", 0);
	define<decoration_object>(DECORATION_PLATFORM_BOTTOM, "Platform bottom", 0);
	define<decoration_object>(DECORATION_PLATFORM_LOG, "Platform log", 0);
	define<decoration_object>(DECORATION_PLATFORM_BOARD_1, "Platform board (1)", 0);
	define<decoration_object>(DECORATION_PLATFORM_BOARD_2, "Platform board (2)", 0);
	define<decoration_object>(DECORATION_PLATFORM_BOARD_3, "Platform board (3)", 0);
	define<decoration_object>(DECORATION_LOG_HOLLOW_FRONT, "Log hollow front", 0);
	define<decoration_object>(DECORATION_LOG_HOLLOW_BACK, "Log hollow back", 0);
	define<decoration_object>(DECORATION_SAFEHOUSE_WALL, "Safe house wall", 0);

	// Items
	define<item_object>(ITEM_COIN, "Coin", 0);
	define<item_object>(ITEM_CHERRY, "Cherry", 0);
	define<item_object>(ITEM_MANDARIN, "Mandarin", 0);
	define<item_object>(ITEM_PEAR, "Pear", 0);
	define<item_object>(ITEM_STRAWBERRY, "Strawberry", 0);

	// Platforms
	define<platform_object>(0, "Platform", 0);

	// Doors
	define<door_object>(DOOR_REGULAR, "Regular door", 0);
	define<door_object>(DOOR_YELLOW, "Yellow door", 0);
	define<door_object>(DOOR_RED, "Red door", 0);

	// Put into categories
	categories.push_back("Player");
	categories.push_back("Chaser");
	categories.push_back("Decoration");
	categories.push_back("Item");
	categories.push_back("Platform");
	categories.push_back("Door");
	category_objects[OBJECT_TYPE_PLAYER].push_back({ OBJECT_TYPE_PLAYER, 0 });
	for (int i = 0; i < TOTAL_CHASERS; i++) {
		category_objects[OBJECT_TYPE_CHASER].push_back({ OBJECT_TYPE_CHASER, i });
	}
	for (int i = 0; i < TOTAL_DECORATIONS; i++) {
		category_objects[OBJECT_TYPE_DECORATION].push_back({ OBJECT_TYPE_DECORATION, i });
	}
	for (int i = 0; i < TOTAL_ITEMS; i++) {
		category_objects[OBJECT_TYPE_ITEM].push_back({ OBJECT_TYPE_ITEM, i });
	}
	category_objects[OBJECT_TYPE_PLATFORM].push_back({ OBJECT_TYPE_PLATFORM, 0 });
	for (int i = 0; i < TOTAL_DOORS; i++) {
		category_objects[OBJECT_TYPE_DOOR].push_back({ OBJECT_TYPE_DOOR, i });
	}
}
