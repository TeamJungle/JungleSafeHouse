#include "npc_object.hpp"
#include "assets.hpp"
#include "game.hpp"
#include "world.hpp"

npc_object::npc_object() {

}

void npc_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	ne::game_object::update(world, chunk);
}

void npc_object::draw() {
	textures.objects.npc[subtype].bind();
	ne::shader::set_transform(&transform);
	animated_quad().bind();
	animation.draw(false);
}

void npc_object::write(ne::memory_buffer* buffer) {
	ne::game_object::write(buffer);
}

void npc_object::read(ne::memory_buffer* buffer) {
	ne::game_object::read(buffer);
}

void npc_object::open_shop(shop_ui* shop) {
	animation.frame = 2;
	animation.sub_frame = 2.0f;
	shop->is_open = true;
	shop->owner = this;
	ne::ortho_camera::bound()->target_chase_aspect.y = 1.2f;
	settings::play(&audio.monkey, 0.1f);
}

void npc_object::close_shop(shop_ui* shop) {
	animation.frame = 0;
	animation.sub_frame = 0.0f;
	shop->is_open = false;
	shop->owner = nullptr;
	ne::ortho_camera::bound()->target_chase_aspect.y = 1.75f;
}

shop_ui::shop_ui() {
	interact_listener = input().interact.listen([this] {
		if (!is_open) {
			return;
		}
		if (!world->save_data || items[active_slot].is_bought) {
			return;
		}
		if (world->save_data->get_coins() >= items[active_slot].price) {
			world->save_data->add_coins(-items[active_slot].price);
			world->save_data->give_machete();
			items[active_slot].is_bought = true;
		}
	});
	left_listener = input().ui_left.listen([this] {
		if (!is_open) {
			return;
		}
		active_slot--;
		if (active_slot < 0) {
			active_slot = items.size() - 1;
		}
	});
	right_listener = input().ui_right.listen([this] {
		if (!is_open) {
			return;
		}
		active_slot++;
		if (active_slot >= (int)items.size()) {
			active_slot = 0;
		}
	});
	add_item({ 54.0f, 100.0f }, &textures.ui.machete, 100);
	add_item({ 104.0f, 100.0f }, &textures.ui.machete, 400);
	add_item({ 156.0f, 100.0f }, &textures.ui.machete, 1000);
}

shop_ui::~shop_ui() {
	input().interact.erase(&interact_listener);
	input().ui_left.erase(&left_listener);
	input().ui_right.erase(&right_listener);
}

void shop_ui::update() {
	if (!is_open) {
		return;
	}
}

void shop_ui::draw(const ne::transform3f& view) {
	if (!is_open) {
		return;
	}
	ne::ortho_camera* camera = ne::ortho_camera::bound();
	still_quad().bind();
	textures.ui.shop.bind();
	ne::transform3f transform;
	transform.scale.xy = textures.ui.shop.size.to<float>() * 0.2f;
	transform.position.x = view.position.x + camera->width() / 2.0f - transform.scale.width / 2.0f;
	transform.position.y = view.position.y + camera->height() / 2.0f - transform.scale.height / 2.0f;
	ne::shader::set_transform(&transform);
	still_quad().draw();

	for (size_t i = 0; i < items.size(); i++) {
		ne::transform3f i_transform = transform;
		i_transform.position.x += items[i].offset.x;
		i_transform.position.y += items[i].offset.y;
		i_transform.scale.xy = items[i].texture->size.to<float>();
		ne::shader::set_transform(&i_transform);
		items[i].texture->bind();
		if (items[i].is_bought) {
			ne::shader::set_color({ 0.8f, 0.8f, 0.8f, 0.5f });
		}
		still_quad().draw();
		items[i].label.transform.position.x = i_transform.position.x + i_transform.scale.x / 2.0f - items[i].label.transform.scale.x / 2.0f;
		items[i].label.transform.position.y = i_transform.position.y + i_transform.scale.y + 8.0f;
		ne::shader::set_color({ 0.2f, 0.2f, 0.2f, 0.75f });
		items[i].label.draw();
		items[i].label.transform.position.x -= 1.0f;
		items[i].label.transform.position.y -= 2.0f;
		if (i == active_slot) {
			ne::shader::set_color({ 1.0f, 0.8f, 0.5f, 1.0f });
		} else {
			ne::shader::set_color({ 1.0f, 0.8f, 0.5f, 0.5f });
		}
		items[i].label.draw();
		ne::shader::set_color(1.0f);
	}
}

void shop_ui::add_item(ne::vector2f offset, ne::texture* texture, int price) {
	shop_item item;
	item.texture = texture;
	item.offset = offset;
	item.price = price;
	item.label.font = &fonts.shop_price;
	item.label.render(STRING(price));
	items.push_back(item);
}
