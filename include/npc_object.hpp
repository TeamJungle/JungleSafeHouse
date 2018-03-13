#pragma once

#include "game_object_types.hpp"

#include <game_object.hpp>

#define NPC_MONKEY 0
#define TOTAL_NPCS 1

// TODO: NPC and shop are poorly designed.

class shop_ui;
class game_world;

class npc_object : public ne::game_object {
public:

	NE_OBJECT_TYPE(OBJECT_TYPE_NPC, "NPC", false);

	npc_object();

	void open_shop(shop_ui* shop);
	void close_shop(shop_ui* shop);

};

class shop_ui {
public:

	npc_object* owner = nullptr;
	bool is_open = false;
	game_world* world = nullptr;

	shop_ui();
	~shop_ui();

	void update();
	void draw(const ne::transform3f& view);

private:

	struct shop_item {
		ne::vector2f offset;
		ne::texture* texture = nullptr;
		int price = 0;
		ne::font_text label;
		bool is_bought = false;
	};

	int interact_listener = -1;
	int left_listener = -1;
	int right_listener = -1;

	std::vector<shop_item> items;
	int active_slot = 0;

	void add_item(ne::vector2f offset, ne::texture* texture, int price);

};
