#include "assets.hpp"

#include <engine.hpp>

class asset_container {
public:
	texture_assets _textures;
	font_assets _fonts;
	shader_assets _shaders;
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
	// Load all the assets in one go.
	_assets->_textures.load_all();
	_assets->_fonts.load_all();
	_assets->_shaders.load_all();
	// Process the loaded assets. Textures must be rendered, etc...
	_assets->_textures.process_some(1000);
	_assets->_fonts.process_some(1000);
	_assets->_shaders.process_some(1000);
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

void texture_assets::initialize() {
	root("assets/textures");

	load({ &blank, "blank.png" }, false);
	load({ &nothing, "nothing.png" }, false);
	load({ &button, "button.png", 3 });

	group("objects/player");
	load({ &objects.player.idle[0], "idle.png", 1, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.player.idle[1], "idle.png", 1, TEXTURE_IS_ANIMATED });
	load({ &objects.player.run[0], "run.png", 11, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.player.run[1], "run.png", 11, TEXTURE_IS_ANIMATED });
	load({ &objects.player.jump[0], "jump.png", 11, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.player.jump[1], "jump.png", 11, TEXTURE_IS_ANIMATED });


	group("objects/chaser");
	//load({ &objects.chaser.idle[0], "idle.png", 1, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	//load({ &objects.chaser.idle[1], "idle.png", 1, TEXTURE_IS_ANIMATED });
	load({ &objects.chaser.run[0], "run.png", 6, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.chaser.run[1], "run.png", 6, TEXTURE_IS_ANIMATED });
	load({ &objects.chaser.jump[0], "jump.png", 7, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.chaser.jump[1], "jump.png", 7, TEXTURE_IS_ANIMATED });

	group("objects/decoration");
	for (int i = 0; i < TOTAL_DECORATIONS; i++) {
		load({ &objects.decoration[i], STRING(i << ".png") });
	}

	group("objects/slide_under");
	load({ &objects.slide_under.tree, "tree.png" });

	group("bg");
	load({ &bg.bg, "bg.png" });
	load({ &bg.bg_back, "bg_back.png" });
	load({ &bg.bg_bott, "bg_bott.png" });
	load({ &bg.bg_far1, "bg_far1.png" });
	load({ &bg.bg_fog, "bg_fog.png" });
	load({ &bg.bg_mid, "bg_mid.png" });
	load({ &bg.bg_top, "bg_top.png" });
	load({ &bg.bg_top_lines, "bg_top_lines.png" });
	load({ &bg.menu, "menu.png" });
	load({ &bg.popup, "popup.png" });

	spawn_thread();
	finish();
}

void font_assets::initialize() {
	root("assets/fonts");
	load({ &hud, "troika.otf", 42, false });
	load({ &hud_small, "troika.otf", 26, false });
	load({ &debug, "troika.otf", 16, false });
}

void shader_assets::initialize() {
	root("assets/shaders");
	load({ &basic, "basic" });
}
