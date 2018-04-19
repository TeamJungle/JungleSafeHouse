#pragma once

#include <transform.hpp>
#include <asset.hpp>
#include <timer.hpp>
#include <memory.hpp>

class game_world;

class game_world_background {
public:

	bool is_visible = true;

	// Used with sin() to make some slight varying movement.
	ne::timer timer;
	float speed = 0.0f;
	float x_vary = 0.0f;

	// The offset from the top of the view.
	ne::vector2f top_offset;

	// The offset from the bottom of the view.
	ne::vector2f bottom_offset;

	// The scale of the texture.
	float zoom = 1.0f;

	std::string name;

	void draw(game_world* world, const ne::transform3f& view, ne::texture* texture) const;

	void write(ne::memory_buffer* buffer);
	void read(ne::memory_buffer* buffer);

};
