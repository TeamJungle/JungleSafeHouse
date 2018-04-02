#include "game_world_background.hpp"
#include "game.hpp"

void game_world_background::draw(const ne::transform3f& view, ne::texture* texture) const {
	if (!is_visible) {
		return;
	}
	assert(texture);
	ne::transform3f transform;
	transform.scale.xy = texture->size.to<float>() * zoom;
	transform.scale.xy.ceil();
	texture->bind();
	still_quad().bind();
	if ((int)transform.scale.width == 0) {
		return;
	}
	float x_start = view.position.x - (float)((int)view.position.x % (int)transform.scale.width);
	float x_end = view.position.x + view.scale.width;
	if (timer.milliseconds() > 0 && (int)speed != 0) {
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

void game_world_background::write(ne::memory_buffer* buffer) {
	buffer->write_uint8(is_visible ? 1 : 0);
	buffer->write_float(speed);
	buffer->write_float(x_vary);
	buffer->write_vector2f(top_offset);
	buffer->write_vector2f(bottom_offset);
	buffer->write_float(zoom);
	buffer->write_string(name);
}

void game_world_background::read(ne::memory_buffer* buffer) {
	is_visible = (buffer->read_uint8() != 0);
	speed = buffer->read_float();
	x_vary = buffer->read_float();
	top_offset = buffer->read_vector2f();
	bottom_offset = buffer->read_vector2f();
	zoom = buffer->read_float();
	name = buffer->read_string();
	if ((int)speed != 0 && (int)x_vary != 0) {
		timer.start();
	}
}
