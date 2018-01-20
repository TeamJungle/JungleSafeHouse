#pragma once

#include "game_object_types.hpp"

#include <memory.hpp>

class game_object;
class game_world;
class game_state;

class game_object_container {
public:

	game_object_container(game_state* game);

	void update(game_world* world);
	void draw();
	void clear();

	void add(game_object* object);
	bool destroy(int id);

	template<typename T>
	T* find(int id) {
		if (objects.find(id) != objects.end()) {
			return (T*)objects[id];
		}
		return nullptr;
	}

	int id_of(game_object* object) const;

	void write(ne::memory_buffer* buffer);
	void read(ne::memory_buffer* buffer);

private:

	std::unordered_map<int, game_object*> objects;
	int id_counter = 0;
	game_state* game = nullptr;

};
