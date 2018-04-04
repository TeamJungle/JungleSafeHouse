#include "editor.hpp"
#include "game.hpp"
#include "menu.hpp"
#include "assets.hpp"

#include <platform.hpp>

void editor_state::start_drag() {
	if (selected && !select_object_popup.is_open) {
		ne::vector2f camera_mouse = camera.mouse();
		if (!drag.is_dragging) {
			drag.is_dragging = true;
			drag.old_position = selected->transform.position.xy;
			drag.new_position = drag.old_position;
			drag.offset = camera_mouse - drag.old_position;
		}
	}
}

void editor_state::change_world(int level_num) {
#if DEVELOPMENT_EDITOR
	std::string world_path = STRING("worlds/local/" << level_num << ".world");
#else
	std::string world_path = STRING("worlds/" << level_num << ".world");
#endif
	if (ne::file_exists(world_path)) {
		world.load(world_path);
	} else {
		world.reset();
		world.init();
	}
	world.level_num = level_num;
	drag = {};
	selected = nullptr;
	camera.transform.position.xy = 0.0f;
}

int editor_state::next_free_level_num() const {
#if DEVELOPMENT_EDITOR
	int num = 0;
	std::string path_prefix = "worlds/local/";
#else
	int num = 100;
	std::string path_prefix = "worlds/";
#endif
	for (int i = num; i < 1000; i++) {
		if (!ne::file_exists(STRING(path_prefix << i << ".world"))) {
			return i;
		}
	}
	return 1000;
}

editor_state::editor_state() {
	ne::initialize_imgui();
	camera.zoom = 2.0f;

	change_world(0);

	place_meta = world.definitions.objects.meta->get_meta(0, 0);

	key_listener = ne::listen([this](ne::keyboard_key_message key) {
		if (!key.is_pressed) {
			return;
		}
		if (SDL_GetModState() & SDLK_LCTRL) {
			if (key.key == KEY_S) {
				save();
			}
		} else {
			if (key.key == KEY_ESCAPE) {
				if (saved) {
					ne::swap_state<menu_state>();
				} else {
					ne::show_message("Please save first, or exit the game.");
					// TODO: Add a "Don't save" option.
					// It's a bit annoying for now, but better than accidentally losing work.
				}
			}
		}
	});

	click_listener = ne::listen([this](ne::mouse_button_message click) {
		if (ne::mouse_position().x < 256 || any_context_open) {
			return;
		}
		if (click.button != MOUSE_BUTTON_LEFT) {
			return;
		}
		if (!click.is_pressed) {
			if (select_object_popup.is_open || tool != EDITOR_TOOL_SELECT) {
				return;
			}
			if (drag.is_dragging) {
				saved = false;
			}
			drag = {};
			return;
		}
		std::vector<ne::game_object*> objects = world.find_objects_at_position(camera.mouse());
		// The last object is more likely to be the one we want to act on.
		ne::game_object* last = (objects.size() > 0 ? objects.back() : nullptr);
		switch (tool) {
		case EDITOR_TOOL_SELECT: {
			click_to_drag = (selected == last);
			selected = last;
			start_drag();
			break;
		}
		case EDITOR_TOOL_PLACE: {
			ne::texture* texture = ne::game_object_factory::default_texture(place_meta);
			ne::vector2f place_position = camera.mouse();
			ne::vector2i texture_size = texture->frame_size() / 2;
			place_position.x -= (float)texture_size.x * object_scale;
			place_position.y -= (float)texture_size.y * object_scale;
			place_position.x -= (float)((int)place_position.x % grid.x);
			place_position.y -= (float)((int)place_position.y % grid.y);
			place_position.floor();
			auto object = world.spawn(world.definitions.objects.meta->get_meta(place_meta.type, place_meta.subtype), place_position);
			if (object) {
				object->transform.rotation.z = object_rotation;
				object->transform.scale.x *= object_scale;
				object->transform.scale.y *= object_scale;
				if (object->type() == OBJECT_TYPE_DECORATION || object->type() == OBJECT_TYPE_VINE) {
					object->collision.offset = 0.0f;
					object->collision.size = 0.0f;
				} else {
					if (object->type() != OBJECT_TYPE_PLAYER && object->type() != OBJECT_TYPE_CHASER) {
						object->collision.size = object->transform.scale.xy;
					}
				}
			}
			saved = false;
			break;
		}
		case EDITOR_TOOL_REMOVE:
			if (last) {
				world.destroy_object(last->id, nullptr);
				saved = false;
			}
			break;
		default:
			break;
		}
	});

	wheel_listener = ne::listen([this](ne::mouse_wheel_message wheel) {
		if (any_context_open) {
			return;
		}
		if (SDL_GetModState() & SDLK_LCTRL) {
			const float rotate = (float)wheel.wheel.y * 0.05f;
			if (tool == EDITOR_TOOL_PLACE) {
				object_rotation += rotate;
			} else if (tool == EDITOR_TOOL_SELECT && selected) {
				selected->transform.rotation.z += rotate;
				saved = false;
			}
		} else if (SDL_GetModState() & SDLK_LSHIFT) {
			if (tool == EDITOR_TOOL_PLACE) {
				object_scale += (float)wheel.wheel.y * 0.1f;
				if (object_scale < 0.1f) {
					object_scale = 0.1f;
				}
			} else if (tool == EDITOR_TOOL_SELECT && selected) {
				selected->transform.scale.x += (float)wheel.wheel.y * 2.0f;
				selected->transform.scale.y += (float)wheel.wheel.y * 2.0f;
				if (selected->transform.scale.x < 2.0f) {
					selected->transform.scale.x = 2.0f;
				}
				if (selected->transform.scale.y < 2.0f) {
					selected->transform.scale.y = 2.0f;
				}
				saved = false;
			}
		}
	});

	world.draw_collisions = true;
}

editor_state::~editor_state() {
	ne::erase(&key_listener);
	ne::erase(&click_listener);
	ne::erase(&wheel_listener);
	ne::stop_imgui();
}

void editor_state::update() {
	camera.transform.scale.xy = ne::window_size().to<float>();
	ui_camera.transform.scale.xy = ne::window_size().to<float>();

	camera.update();

	if (!(SDL_GetModState() & SDLK_LCTRL)) {
		const float camera_speed = 20.0f;
		if (ne::is_key_down(KEY_A) || ne::is_key_down(KEY_LEFT)) {
			camera.transform.position.x -= camera_speed;
		}
		if (ne::is_key_down(KEY_D) || ne::is_key_down(KEY_RIGHT)) {
			camera.transform.position.x += camera_speed;
		}
		if (ne::is_key_down(KEY_W) || ne::is_key_down(KEY_UP)) {
			camera.transform.position.y -= camera_speed;
		}
		if (ne::is_key_down(KEY_S) || ne::is_key_down(KEY_DOWN)) {
			camera.transform.position.y += camera_speed;
		}
		if (camera.transform.position.x < -left_panel.scale.width) {
			camera.transform.position.x = -left_panel.scale.width;
		}
		if (camera.transform.position.y < -300.0f) {
			camera.transform.position.y = -300.0f;
		}
	}

	// ImGui
	any_context_open = false;
	left_panel.scale.width = 256.0f;
	left_panel.scale.height = ui_camera.transform.scale.height - 8.0f;
	left_panel.position.xy = 0.0f;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.1f, 0.4f, 0.1f, 0.98f });
	ImGui::SetNextWindowPos({ left_panel.position.x, left_panel.position.y }, ImGuiSetCond_Once);
	ImGui::SetNextWindowSize({ left_panel.scale.width, left_panel.scale.height }, ImGuiSetCond_Once);
	ImGui::Begin("Actions", nullptr,
		ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoTitleBar
	);
	ImGui::PushItemWidth(-1);

	// Level management
	if (ImGui::CollapsingHeader("Load level")) {
		if (level_num == -1) {
			level_num = next_free_level_num();
		}
		if (ImGui::Button("Load##LoadLevel")) {
			change_world(level_num);
		}
		ImGui::SameLine();
		ImGui::InputInt("Level##LevelToLoad", &level_num);
	}
	ImGui::Separator();

	// General info
	ImGui::Text(CSTRING("Saved: " << (saved ? "Yes" : "No ")));
	ImGui::SameLine();
	ImVec4 col_good = { 0.2f, 1.0f, 0.4f, 0.7f };
	ImVec4 col_edit = { 0.8f, 0.8f, 0.1f, 0.7f };
	ImVec4 col_now = (saved ? col_good : col_edit);
	ImVec4 col_border = { col_now.x + 0.2f, col_now.y + 0.2f, col_now.z + 0.2f, col_now.w };
	ImGui::Image((ImTextureID)textures.blank.id, { 10.0f, 10.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, col_now, col_border);
	if (ImGui::CollapsingHeader("General", nullptr, true, true)) {
		ImGui::Text("Level Number: ");
		ImGui::SameLine();
		ImGui::Text(CSTRING(world.level_num));
#if !DEVELOPMENT_EDITOR
		// Players should not be able to edit the safehouse or built-in levels.
		if (world.level_num < 100) {
			world.level_num = 100;
		}
#endif
		int ground_y_int = (int)world.ground_y;
		ImGui::Text("Ground: ");
		ImGui::SameLine();
		ImGui::InputInt("Ground", &ground_y_int);
		if (ground_y_int < 1) {
			ground_y_int = 1;
		}
		world.ground_y = (float)ground_y_int;
		ImGui::Checkbox("Draw collisions", &world.draw_collisions);
		ImGui::Text(CSTRING("Objects: " << world.object_count()));
		ImGui::SameLine();
		ImGui::Text(CSTRING("Chunks: " << world.chunks.size()));
		ImGui::Text("Grid:");
		ImGui::SameLine();
		ImGui::InputInt2("Grid", &grid.x);
		if (grid.x < 1) {
			grid.x = 1;
		} else if (grid.x > 1000) {
			grid.x = 1000;
		}
		if (grid.y < 1) {
			grid.y = 1;
		} else if (grid.y > 1000) {
			grid.y = 1000;
		}
		ImGui::Text("Light");
		ImGui::InputFloat("Base", &world.base_light);
		if (world.base_light < 0.0f) {
			world.base_light = 0.0f;
		} else if (world.base_light > 1.0f) {
			world.base_light = 1.0f;
		}
	}

	// Backgrounds
	if (ImGui::CollapsingHeader("Backgrounds##WorldBackgrounds")) {
		auto edit_background = [](game_world_background* background) {
			if (ImGui::CollapsingHeader(background->name.c_str(), nullptr, true, false)) {
				ImGui::Checkbox(CSTRING("Visible##BgVisible" << background->name), &background->is_visible);
				ImGui::Text("Top");
				ImGui::SameLine();
				ImGui::InputFloat2(CSTRING("Top##BgTop" << background->name), &background->top_offset.x);
				ImGui::Text("Bottom");
				ImGui::SameLine();
				ImGui::InputFloat2(CSTRING("Bottom##BgBottom" << background->name), &background->bottom_offset.x);
				ImGui::Text("Zoom");
				ImGui::SameLine();
				ImGui::InputFloat(CSTRING("Zoom##BgZoom" << background->name), &background->zoom);
				ImGui::Text("Speed");
				ImGui::SameLine();
				ImGui::InputFloat(CSTRING("Speed##BgSpeed" << background->name), &background->speed);
				ImGui::Text("X Vary");
				ImGui::SameLine();
				ImGui::InputFloat(CSTRING("X Vary##BgXVary" << background->name), &background->x_vary);
			}
		};
		edit_background(&world.backgrounds.background);
		edit_background(&world.backgrounds.trees);
		edit_background(&world.backgrounds.fog_back);
		edit_background(&world.backgrounds.mid);
		edit_background(&world.backgrounds.top);
		edit_background(&world.backgrounds.top_lines);
		edit_background(&world.backgrounds.bottom);
		edit_background(&world.backgrounds.fog_front);
		ImGui::Separator();
		if (ImGui::Button("Reset##ResetBgDefaults")) {
			world.backgrounds = {};
			world.backgrounds.set_default();
		}
	}

	// Tools
	ImGui::Text("Tool");
	const int old_tool = tool;
	ImGui::RadioButton("Select", &tool, EDITOR_TOOL_SELECT);
	ImGui::RadioButton("Place", &tool, EDITOR_TOOL_PLACE);
	ImGui::RadioButton("Remove", &tool, EDITOR_TOOL_REMOVE);
	if (old_tool != tool) {
		selected = nullptr;
		object_rotation = 0.0f;
		object_scale = 1.0f;
	}
	ImGui::Separator();

	// Active tool
	switch (tool) {
	case EDITOR_TOOL_SELECT: {
		if (ImGui::IsMouseClicked(1) && ne::mouse_position().x > 256 && !drag.is_dragging) {
			select_object_popup.objects = world.find_objects_at_position(camera.mouse());
			if (select_object_popup.objects.size() > 0) {
				ImGui::OpenPopup("SelectObjectPopupMenu");
			}
		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		select_object_popup.is_open = false;
		if (ImGui::BeginPopup("SelectObjectPopupMenu")) {
			select_object_popup.is_open = true;
			for (auto& i : select_object_popup.objects) {
				auto i_meta = world.definitions.objects.meta->get_meta(i->type(), i->subtype);
				if (ImGui::MenuItem(STRING(i_meta.name << " (" << i->type_name() << ")" << " #" << i->id).c_str())) {
					selected = i;
					start_drag();
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();

		ImGui::Text("Selected object: ");
		ImGui::SameLine();
		if (!selected) {
			ImGui::Text("None.");
			break;
		}
		ImGui::Text(CSTRING(selected->type_name() << " (" << selected->id << ")"));
		if (ImGui::Button("Unselect")) {
			selected = nullptr;
			break;
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove this")) {
			world.destroy_object(selected->id, nullptr);
			selected = nullptr;
			break;
		}
		ImGui::Text(CSTRING("Position: " << selected->transform.position.xy));
		ImGui::Text("Z: ");
		ImGui::SameLine();
		ImGui::InputInt("ZPosition", &selected->draw_order);
		float scale[2] = { selected->transform.scale.width, selected->transform.scale.height };
		ImGui::InputFloat2("Size", &scale[0], 1);
		selected->transform.scale.xy = { scale[0], scale[1] };
		ImGui::SameLine();
		if (ImGui::Button("Reset##ResetSize")) {
			auto selected_meta = world.definitions.objects.meta->get_meta(selected->type(), selected->subtype);
			selected->transform.scale.xy = ne::game_object_factory::default_texture(selected_meta)->frame_size().to<float>();
		}
		ImGui::Text(CSTRING("Rotation: " << selected->transform.rotation.z));
		ImGui::SameLine();
		if (ImGui::Button("Reset##ResetRotation")) {
			selected->transform.rotation.z = 0.0f;
		}
		ImGui::Separator();
		ImGui::Text("Collision");
		ImGui::SameLine();
		if (ImGui::Button("Reset##ResetCollision")) {
			selected->collision.offset = 0.0f;
			selected->collision.size = selected->transform.scale.xy;
		}
		ImGui::SameLine();
		if (ImGui::Button("None##NoCollision")) {
			selected->collision.offset = 0.0f;
			selected->collision.size = 0.0f;
		}
		int collision[4];
		collision[0] = (int)selected->collision.offset.x;
		collision[1] = (int)selected->collision.offset.y;
		collision[2] = (int)selected->collision.size.width;
		collision[3] = (int)selected->collision.size.height;
		ImGui::InputInt4("Collision", &collision[0]);
		selected->collision.offset = { (float)collision[0], (float)collision[1] };
		selected->collision.size = { (float)collision[2], (float)collision[3] };
		ImGui::Separator();
		ImGui::Text("Light: ");
		ImGui::SameLine();
		if (world.lights.size() >= 20) {
			ImGui::Text("Cannot add light to this object. Limit reached.");
		} else {
			bool has_light = false;
			for (auto& i : world.lights) {
				if (i.object_id == selected->id) {
					ImGui::ColorEdit3("Color", &i.color.x);
					ImGui::Text("Intensity: ");
					ImGui::SameLine();
					ImGui::InputFloat("Intensity", &i.intensity);
					ImGui::Text("Rotate speed: ");
					ImGui::SameLine();
					ImGui::InputFloat("Rotate speed", &i.rotate_speed);
					ImGui::Text("Rotate distance: ");
					ImGui::SameLine();
					ImGui::InputFloat("Rotate distance", &i.rotate_distance);
					has_light = true;
					break;
				}
			}
			if (!has_light) {
				if (ImGui::Button("Add##AddLight")) {
					world.lights.push_back({});
					world.lights.back().object_id = selected->id;
				}
			} else {
				if (ImGui::Button("Remove##RemoveLight")) {
					for (size_t i = 0; i < world.lights.size(); i++) {
						if (world.lights[i].object_id == selected->id) {
							world.lights.erase(world.lights.begin() + i);
							break;
						}
					}
				}
			}
		}
		ImGui::Separator();
		if (selected->type() == OBJECT_TYPE_DOOR) {
			auto door = (door_object*)selected;
			ImGui::Text("Level: ");
			ImGui::SameLine();
			ImGui::InputInt("Leads to level", &door->leads_to_level_num);
			if (door->leads_to_level_num < 0) {
				door->leads_to_level_num = 0;
			}
			ImGui::Text("Cost: ");
			ImGui::SameLine();
			ImGui::InputInt("Cost to open", &door->opening_cost);

			ImGui::Text("Gem cost: ");
			ImGui::SameLine();
			ImGui::InputInt("Gem cost to open", &door->opening_gem_cost);

		} else if (selected->type() == OBJECT_TYPE_DECORATION) {
			auto decoration = (decoration_object*)selected;
			ImGui::Checkbox("Flip X", &decoration->flip_x);
			ImGui::Checkbox("Flip Y", &decoration->flip_y);
		} else if (selected->type() == OBJECT_TYPE_VINE) {
			auto vine = (decoration_object*)selected;
			ImGui::Checkbox("Flip X", &vine->flip_x);
		}
		break;
	}
	case EDITOR_TOOL_PLACE: {
		ImGui::Text(CSTRING("Rotation: " << object_rotation));
		ImGui::SameLine();
		if (ImGui::Button("Reset##ResetPlaceRotation")) {
			object_rotation = 0.0f;
		}
		ImGui::Text(CSTRING("Scale: " << object_scale));
		ImGui::SameLine();
		if (ImGui::Button("Reset##ResetPlaceScale")) {
			object_scale = 1.0f;
		}
		ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f, 1.0f, 1.0f, 0.25f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 1.0f, 1.0f, 0.5f });
		ImGui::Button(CSTRING(place_meta.name << "##SelectedPlaceObject"), { 128.0f, 32.0f });
		auto meta = world.definitions.objects.meta;
		if (ImGui::BeginPopupContextItem("PlaceObject")) {
			any_context_open = true;
			for (int i = 0; i < TOTAL_OBJECT_TYPES; i++) {
				if (ImGui::BeginMenu(CSTRING(meta->get_category_name(i) << "##TypeMenu" << i << "PlaceObject"))) {
					auto objects = meta->get_meta_in_category(i);
					for (auto& k : objects) {
						if (ImGui::MenuItem(CSTRING(k.name << "##SubtypeMenuPlaceObject"))) {
							place_meta = k;
						}
					}
					ImGui::EndMenu();
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor(2);
		break;
	}
	case EDITOR_TOOL_REMOVE:

		break;
	default:
		break;
	}

	ImGui::PopItemWidth();
	ImGui::End();
	ImGui::PopStyleColor();

	if (drag.is_dragging && click_to_drag && ne::is_mouse_button_down(MOUSE_BUTTON_LEFT)) {
		ne::vector2f mouse = camera.mouse();
		mouse.floor();
		drag.new_position = {
			mouse.x - drag.offset.x,
			mouse.y - drag.offset.y
		};
		selected->transform.position.xy = {
			drag.new_position.x - (float)((int)drag.new_position.x % grid.x),
			drag.new_position.y - (float)((int)drag.new_position.y % grid.y)
		};
		selected->transform.position.floor();
	}

	// Update debug info
	debug.set(&fonts.debug, STRING(
		"Delta " << ne::delta() << 
		"\nFPS: " << ne::current_fps()
	));
}

void editor_state::draw() {
	shaders.basic.bind();
	ne::transform3f view;
	
	// World
	ne::shader::set_color(1.0f);
	camera.bind();
	view.position.xy = camera.xy();
	view.scale.xy = camera.size();
	world.draw(view);

	switch (tool) {
	case EDITOR_TOOL_PLACE: {
		ne::shader::set_color(1.0f);
		ne::transform3f meta_transform;
		meta_transform.position.xy = camera.mouse();
		ne::texture* texture = ne::game_object_factory::default_texture(place_meta);
		ne::vector2i texture_size = texture->frame_size() / 2;
		meta_transform.position.x -= (float)texture_size.x * object_scale;
		meta_transform.position.y -= (float)texture_size.y * object_scale;
		meta_transform.position.x -= (float)((int)meta_transform.position.x % grid.x);
		meta_transform.position.y -= (float)((int)meta_transform.position.y % grid.y);
		meta_transform.position.floor();
		meta_transform.scale.xy = object_scale;
		meta_transform.rotation.z = object_rotation;
		animated_quad().bind();
		place_meta.draw(meta_transform, 0.75f);
		break;
	}
	default:
		break;
	}

	// UI
	ne::shader::set_color(1.0f);
	still_quad().bind();
	ui_camera.bind();
	view.position.xy = ui_camera.xy();
	view.scale.xy = ui_camera.size();
	debug.draw(view);
}

void editor_state::save() {
#if DEVELOPMENT_EDITOR
	std::string path = STRING("worlds/local/" << world.level_num << ".world");
#else
	std::string path = STRING("worlds/" << world.level_num << ".world");
#endif
	world.save(path);
	saved = true;
}
