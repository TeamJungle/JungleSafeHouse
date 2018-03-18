#include "button_menu.hpp"
#include "assets.hpp"
#include "game.hpp"

#include <input.hpp>

void button_menu::update(const ne::vector2f& camera_size, const ne::vector2f& position, const ne::vector2f& size) {
	ne::vector2f margin = {
		0.0f,
		camera_size.height / 2.0f - size.height / 2.0f
	};
	int index = 0;
	for (auto& i : buttons) {
		i.transform.position.xy = {
			position.x + camera_size.width / 2.0f - i.transform.scale.width / 2.0f,
			position.y + i.transform.scale.height * 1.3f
		};
		if (is_horizontal) {
			i.transform.position.x += margin.x;
		} else {
			i.transform.position.y += margin.y;
		}
		i.update();
		if (index == active_index) {
			i.label_color = { 0.2f, 0.5f, 0.2f };
		} else if (i.transform.collides_with(ne::mouse_position().to<float>())) {
			i.label_color = { 0.7f, 1.0f, 0.5f };
		} else {
			i.label_color = 1.0f;
		}
		margin.x += i.transform.scale.width * 1.1f;
		margin.y += i.transform.scale.height * 1.3f;
		index++;
	}
}

void button_menu::draw(const ne::vector2f& camera_size, const ne::vector2f& position, const ne::vector2f& size) {
	still_quad().bind();
	ne::shader::set_color(1.0f);
	if (has_background) {
		textures.ui.popup.bind();
		ne::transform3f bg;
		bg.position.xy = position + camera_size / 2.0f - size / 2.0f;
		bg.scale.xy = size;
		ne::shader::set_transform(&bg);
		still_quad().draw();
	}
	for (auto& i : buttons) {
		i.draw();
	}
}

int button_menu::add_button(const std::string& text, const std::function<void()>& action) {
	buttons.emplace_back(ne::ui_button());
	buttons.back().label.font = font;
	buttons.back().label.render(text);
	buttons.back().sprite = &textures.ui.button;
	buttons.back().transform.scale.width = textures.ui.button.size.width / button_downscale.width;
	buttons.back().transform.scale.height = textures.ui.button.size.height / button_downscale.height;
	buttons.back().button_shape = &animated_quad();
	buttons.back().label_shape = &still_quad();
	buttons.back().click.listen(action);
	return buttons.size() - 1;
}

void button_menu::disable() {
	for (auto& i : buttons) {
		i.click.is_enabled = false;
	}
}

void button_menu::enable() {
	for (auto& i : buttons) {
		i.click.is_enabled = true;
	}
}

void button_menu::set_active(int index) {
	active_index = index;
}
