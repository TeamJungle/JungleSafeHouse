#pragma once

#include "decoration_object.hpp"

#include <asset.hpp>

void load_assets();
void destroy_assets();

class texture_assets : public ne::texture_group {
public:

	ne::texture blank;
	ne::texture nothing;

	struct {
		struct {
			ne::texture idle[2];
			ne::texture run[2];
			ne::texture jump[2];
		} player;
		struct {
			ne::texture idle[2];
			ne::texture run[2];
			ne::texture jump[2];
		} chaser;
		ne::texture decoration[TOTAL_DECORATIONS];
		struct {
			ne::texture tree;
		} slide_under;
	} objects;

	struct {
		ne::texture menu;
		ne::texture popup;

		ne::texture bg;
		ne::texture bg_back;
		ne::texture bg_bott;
		ne::texture bg_far1;
		ne::texture bg_fog;
		ne::texture bg_mid;
		ne::texture bg_top;
		ne::texture bg_top_lines;
	} bg;

	ne::texture button;

	void initialize();

};

class font_assets : public ne::font_group {
public:

	ne::font hud;
	ne::font debug;

	void initialize();

};

class shader_assets : public ne::shader_group {
public:

	ne::shader basic;

	void initialize();

};

texture_assets& _textures();
#define textures _textures()

font_assets& _fonts();
#define fonts _fonts()

shader_assets& _shaders();
#define shaders _shaders()
