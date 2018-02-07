#include "game_world_background.hpp"
#include "game.hpp"

void game_world_background::draw(const ne::transform3f& view, ne::texture* texture) const {
	assert(texture);
	ne::transform3f transform;
	transform.scale.xy = texture->size.to<float>() * zoom;
	texture->bind();
	still_quad().bind();
	float x_start = view.position.x - (float)((int)view.position.x % (int)transform.scale.width);
	float x_end = view.position.x + view.scale.width;
	if (timer.milliseconds() > 0) {
		x_start += x_vary * std::sin((float)timer.milliseconds() / speed);
	}
	for (float x = x_start; x < x_end; x += transform.scale.width) {
		transform.position.x = x + top_offset.x;
		transform.position.y = top_offset.y;
		if (bottom_offset.y > 1.0f) {
			transform.position.y += view.scale.height - bottom_offset.y;
		}
		ne::shader::set_transform(&transform);
		still_quad().draw();
	}
}
