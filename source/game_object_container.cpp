#include "game_object_container.hpp"
#include "game_world.hpp"
#include "game_object.hpp"
#include "game_object_types.hpp"

#include <memory.hpp>

game_object_container::game_object_container(game_state* game) : game(game) {
	
}

void game_object_container::update(game_world* world) {
	for (auto& i : objects) {
		i.second->update(world);
	}
}

void game_object_container::draw() {
	for (auto& i : objects) {
		i.second->draw();
	}
}

void game_object_container::clear() {
	for (auto& i : objects) {
		delete i.second;
	}
	objects.clear();
	id_counter = 0;
}

void game_object_container::add(game_object* object) {
	if (id_of(object) != -1) {
		NE_WARNING("Object already exists.");
		return;
	}
	objects[id_counter] = object;
	id_counter++;
}

bool game_object_container::destroy(int id) {
	if (objects.find(id) == objects.end()) {
		return false;
	}
	delete objects[id];
	objects.erase(id);
	return true;
}

int game_object_container::id_of(game_object* object) const {
	for (auto& i : objects) {
		if (i.second == object) {
			return i.first;
		}
	}
	return -1;
}

void game_object_container::write(ne::memory_buffer* buffer) {
	buffer->write_uint32(objects.size());
	for (auto& i : objects) {
		buffer->write_int32((int32)i.second->type());
		i.second->write(buffer);
	}
}

void game_object_container::read(ne::memory_buffer* buffer) {
	clear();
	uint32 object_count = buffer->read_uint32();
	for (uint32 i = 0; i < object_count; i++) {
		game_object_type type = (game_object_type)buffer->read_int32();
		game_object* object = create_object(game, type);
		if (!object) {
			NE_WARNING("Could not create object of type " << (int32)type);
			continue;
		}
		object->read(buffer);
		add(object);
	}
}
