#pragma once

#include "decoration_object.hpp"
#include "item_object.hpp"
#include "door_object.hpp"
#include "spikes_object.hpp"
#include "npc_object.hpp"

#include <asset.hpp>
#include <audio.hpp>

void load_assets();
void destroy_assets();

class texture_assets : public ne::texture_group {
public:

	ne::texture blank;
	ne::texture nothing;
	ne::texture button;
	ne::texture coin;
	ne::texture machete;

	struct {
		struct {
			ne::texture idle[2];
			ne::texture run[2];
			ne::texture jump[2];
			ne::texture slide[2];
			ne::texture flip[2];
		} player, machete;
		struct {
			ne::texture idle[2];
			ne::texture run[2];
			ne::texture jump[2];
		} chaser;
		ne::texture decoration[TOTAL_DECORATIONS];
		ne::texture item[TOTAL_ITEMS];
		struct {
			ne::texture open;
			ne::texture door[TOTAL_DOORS];
		} door;
		ne::texture spikes[TOTAL_SPIKES];
		ne::texture npc[TOTAL_NPCS];
	} objects;

	struct {
		ne::texture menu;
		ne::texture popup;
		ne::texture shop;

		ne::texture bg;
		ne::texture bg_back;
		ne::texture bg_bott;
		ne::texture bg_far1;
		ne::texture bg_fog;
		ne::texture bg_mid;
		ne::texture bg_top;
		ne::texture bg_top_lines;
	} bg;

	void initialize();

};

class font_assets : public ne::font_group {
public:

	ne::font hud;
	ne::font hud_small;
	ne::font shop_price;
	ne::font debug;

	void initialize();

};

class shader_assets : public ne::shader_group {
public:

	ne::shader basic;
	ne::shader light;

	void initialize();

};

class music_assets : public ne::music_group {
public:

	ne::music safehouse;
	ne::music jungle;

	void initialize();

};

class sound_assets : public ne::music_group {
public:

	ne::sound pickup[5];

	void initialize();

};

texture_assets& _textures();
#define textures _textures()

font_assets& _fonts();
#define fonts _fonts()

shader_assets& _shaders();
#define shaders _shaders()

music_assets& _music();
#define music _music()

sound_assets& _sounds();
#define sounds _sounds()
