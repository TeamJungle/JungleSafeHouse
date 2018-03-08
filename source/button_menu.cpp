#include "button_menu.hpp"
#include "assets.hpp"
#include "game.hpp"

#include <input.hpp>

void button_menu::update(const ne::vector2f& camera_size, const ne::vector2f& position, const ne::vector2f& size) {
	float margin_y = camera_size.height / 2.0f - size.height / 2.0f;
	for (auto& i : buttons) {
		i.transform.position.xy = {
			position.x + camera_size.width / 2.0f - i.transform.scale.width / 2.0f,
			position.y + margin_y + i.transform.scale.height * 1.3f
		};
		i.update();
		if (i.transform.collides_with(ne::mouse_position().to<float>())) {
			i.label_color = { 0.7f, 1.0f, 0.5f };
		} else {
			i.label_color = 1.0f;
		}
		margin_y += i.transform.scale.height * 1.3f;
	}
}

void button_menu::draw(const ne::vector2f& camera_size, const ne::vector2f& position, const ne::vector2f& size) {
	still_quad().bind();
	ne::shader::set_color(1.0f);
	if (has_background) {
		textures.bg.popup.bind();
		ne::transform3f bg;
		bg.scale.xy = size;
		bg.position.x = position.x + camera_size.width / 2.0f - bg.scale.width / 2.0f;
		bg.position.y = position.y + camera_size.height / 2.0f - bg.scale.height / 2.0f;
		ne::shader::set_transform(&bg);
		still_quad().draw();
	}
	for (auto& i : buttons) {
		i.draw();
	}
}

void button_menu::add_button(const std::string& text, const std::function<void()>& action) {
	buttons.emplace_back(ne::ui_button());
	buttons.back().label.font = font;
	buttons.back().label.render(text);
	buttons.back().sprite = &textures.button;
	buttons.back().transform.scale.width = textures.button.size.width / button_downscale.width;
	buttons.back().transform.scale.height = textures.button.size.height / button_downscale.height;
	buttons.back().button_shape = &animated_quad();
	buttons.back().label_shape = &still_quad();
	buttons.back().click.listen(action);
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
