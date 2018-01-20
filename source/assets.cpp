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

	group("objects/player");
	load({ &objects.player.idle[0], "idle.png", 1, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.player.idle[1], "idle.png", 1, TEXTURE_IS_ANIMATED });
	load({ &objects.player.run[0], "run.png", 12, TEXTURE_IS_ANIMATED | TEXTURE_FLIP_X });
	load({ &objects.player.run[1], "run.png", 12, TEXTURE_IS_ANIMATED });

	spawn_thread();
	finish();
}

void font_assets::initialize() {
	root("assets/fonts");
	load({ &hud, "jsh.ttf", 14, false });
}

void shader_assets::initialize() {
	root("assets/shaders");
	load({ &basic, "basic" });
}
