#include "assets.hpp"

#include <engine.hpp>

struct asset_container {
	texture_assets _textures;
	font_assets _fonts;
	shader_assets _shaders;
	audio_assets _audio;
};

static asset_container* _assets = nullptr;

void load_assets() {
	if (_assets) {
		NE_WARNING("Already exists");
		return;
	}
	_assets = new asset_container();
	// Prepare information about how to load the assets.
	_assets->_textures.initialize();
	_assets->_fonts.initialize();
	_assets->_shaders.initialize();
	_assets->_audio.initialize();
	// Load all the assets in one go.
	_assets->_textures.load_all();
	_assets->_fonts.load_all();
	_assets->_shaders.load_all();
	_assets->_audio.load_all();
	// Process the loaded assets. Textures must be rendered, etc...
	_assets->_textures.process_some(1000);
	_assets->_fonts.process_some(1000);
	_assets->_shaders.process_some(1000);
	_assets->_audio.process_some(1000);
}

void destroy_assets() {
	if (!_assets) {
		return;
	}
	delete _assets;
	_assets = nullptr;
}

texture_assets& _textures() {
	return _assets->_textures;
}

font_assets& _fonts() {
	return _assets->_fonts;
}

shader_assets& _shaders() {
	return _assets->_shaders;
}

audio_assets& _audio() {
	return _assets->_audio;
}

void texture_assets::initialize() {
	// To make loading a bit more readable.
	const int left = (int)ne::direction_side::left;
	const int right = (int)ne::direction_side::right;

	root("assets/textures");

	load({ &blank, "blank.png" }, false);
	load({ &nothing, "nothing.png" }, false);
	
	load({ &rain, "rain.png" });

	group("objects/player");
	load({ &objects.player.idle[left], "idle.png", 1, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.player.idle[right], "idle.png", 1, TEXTURE_IS_ANIMATED });
	load({ &objects.player.run[left], "run.png", 12, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.player.run[right], "run.png", 12, TEXTURE_IS_ANIMATED });
	load({ &objects.player.jump[left], "jump.png", 11, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.player.jump[right], "jump.png", 11, TEXTURE_IS_ANIMATED });
	load({ &objects.player.slide[left], "slide.png", 11, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.player.slide[right], "slide.png", 11, TEXTURE_IS_ANIMATED });
	load({ &objects.player.flip[left], "flip.png", 8, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.player.flip[right], "flip.png", 8, TEXTURE_IS_ANIMATED });

	group("objects/player/machete");
	load({ &objects.machete.idle[left], "idle.png", 1, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.machete.idle[right], "idle.png", 1, TEXTURE_IS_ANIMATED });
	load({ &objects.machete.run[left], "run.png", 12, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.machete.run[right], "run.png", 12, TEXTURE_IS_ANIMATED });
	load({ &objects.machete.jump[left], "jump.png", 11, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.machete.jump[right], "jump.png", 11, TEXTURE_IS_ANIMATED });
	load({ &objects.machete.slide[left], "slide.png", 11, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.machete.slide[right], "slide.png", 11, TEXTURE_IS_ANIMATED });
	// TODO: Flip animation for machete.
	load({ &objects.machete.flip[left], "jump.png", 11, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.machete.flip[right], "jump.png", 11, TEXTURE_IS_ANIMATED });

	group("objects/chaser");
	load({ &objects.chaser.idle[left], "idle.png", 1, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.chaser.idle[right], "idle.png", 1, TEXTURE_IS_ANIMATED });
	load({ &objects.chaser.run[left], "run.png", 6, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.chaser.run[right], "run.png", 6, TEXTURE_IS_ANIMATED });
	load({ &objects.chaser.jump[left], "jump.png", 7, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.chaser.jump[right], "jump.png", 7, TEXTURE_IS_ANIMATED });

	group("objects/decoration");
	for (int i = 0; i < TOTAL_DECORATIONS; i++) {
		load({ &objects.decoration[i], STRING(i << ".png") });
	}

	group("objects/item");
	load({ &objects.item[ITEM_COIN], "coin.png", 6, TEXTURE_IS_ANIMATED });
	load({ &objects.item[ITEM_CHERRY], "cherry.png" });
	load({ &objects.item[ITEM_MANDARIN], "mandarin.png" });
	load({ &objects.item[ITEM_PEAR], "pear.png" });
	load({ &objects.item[ITEM_STRAWBERRY], "strawberry.png" });
	load({ &objects.item[ITEM_GEM], "gem.png",1 });


	group("objects/door");
	load({ &objects.door.open, "open.png" });
	for (int i = 0; i < TOTAL_DOORS; i++) {
		load({ &objects.door.door[i], STRING(i << ".png") });
	}

	group("objects/spikes");
	for (int i = 0; i < TOTAL_SPIKES; i++) {
		load({ &objects.spikes[i], STRING(i << ".png") });
	}

	group("objects/npc");
	load({ &objects.npc[NPC_MONKEY], "monkey.png", 3 });

	group("objects/vine");
	for (int i = 0; i < TOTAL_VINES; i++) {
		load({ &objects.vine[i], STRING(i << ".png") });
	}

	group("bg");
	load({ &bg.menu, "menu.png" });
	load({ &bg.high_dark, "high_dark.png" });
	load({ &bg.normal_dark, "normal_dark.png" });
	load({ &bg.high_bright, "high_bright2.png" });
	load({ &bg.normal_bright, "normal_bright.png" });
	load({ &bg.bg_back, "bg_back.png" });
	load({ &bg.bg_bott, "bg_bott.png" });
	load({ &bg.bg_far1, "bg_far1.png" });
	load({ &bg.bg_fog, "bg_fog.png" });
	load({ &bg.bg_mid, "bg_mid.png" });
	load({ &bg.bg_top, "bg_top.png" });
	load({ &bg.bg_top_lines, "bg_top_lines.png" });

	group("ui");
	load({ &ui.popup, "popup.png" });
	load({ &ui.shop, "shop.png" });
	load({ &ui.button, "button.png", 3 });
	load({ &ui.coin, "coin.png" });
	load({ &ui.machete, "machete.png" });

	spawn_thread();
	finish();
}

void font_assets::initialize() {
	root("assets/fonts");
	load({ &hud, "troika.otf", 42, false });
	load({ &hud_small, "troika.otf", 26, false });
	load({ &shop_price, "troika.otf", 20, false });
	load({ &debug, "troika.otf", 16, false });
}

void shader_assets::initialize() {
	root("assets/shaders");
	load({ &basic, "basic" });
	load({ &light, "light" });
}

void audio_assets::initialize() {
	root("assets/music");
	load({ &safehouse, "safehouse.ogg", -1 });
	load({ &jungle, "jungle.ogg", -1 });

	root("assets/sounds");
	for (int i = 0; i < 5; i++) {
		load({ &pickup[i], STRING("pickup" << i << ".ogg") });
	}
	load({ &monkey, "monkey.wav" });
	load({ &tiger, "tiger.ogg" });
	load({ &door, "door.ogg" });
	for (int i = 0; i < 3; i++) {
		load({ &vine[i], STRING("vine" << i << ".wav") });
	}
	load({ &rain, "rain.ogg" });
}
