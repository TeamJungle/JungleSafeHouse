#pragma once

#include <transform.hpp>
#include <asset.hpp>
#include <timer.hpp>

class game_world_background {
public:

	bool is_visible = true;

	bool upwards = false;

	// Used with sin() to make some slight varying movement.
	ne::timer timer;
	float speed = 1000.0f;
	float x_vary = 16.0f;

	// The offset from the top of the view.
	ne::vector2f top_offset;

	// The offset from the bottom of the view.
	ne::vector2f bottom_offset;

	// The scale of the texture.
	float zoom = 1.0f;

	void draw(const ne::transform3f& view, ne::texture* texture) const;

};
