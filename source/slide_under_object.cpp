#include "slide_under_object.hpp"
#include "game.hpp"
#include "move.hpp"
#include "player_object.hpp"
#include "assets.hpp"

void slide_under_object::update(ne::game_world* world, ne::game_world_chunk* chunk) {
	ne::transform3f collision_transform = transform;
	collision_transform.position.x += collision_transform.scale.x / 3.0f;
	collision_transform.scale.x /= 3.0f;
	collision_transform.scale.y -= 50.0f;
	world->each<player_object>([&](player_object* player) {
		if (player->transform.collides_with(collision_transform)) {
			player->component<game_object_move_component>()->stop();
		}
	});
}

void slide_under_object::draw() {
	ne::texture* sprite = &textures.objects.slide_under.tree;
	transform.scale.xy = sprite->size.to<float>() / 6.0f;
	transform.rotation.z = -0.3f;
	animated_quad().bind();
	ne::shader::set_transform(&transform);
	sprite->bind();
	animation.draw(true);
}

void slide_under_object::read(ne::memory_buffer* buffer) {

}

void slide_under_object::write(ne::memory_buffer* buffer) {

}
