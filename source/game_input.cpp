#include "game_input.hpp"

game_input::game_input() {
	interact.add_key(KEY_SPACE, true);
	pause.add_key(KEY_ESCAPE, true);

	interact.add_controller_button(0, ne::controller_button::a, true);
	pause.add_controller_button(0, ne::controller_button::guide, true);

	ui_up.add_key(KEY_W, true);
	ui_left.add_key(KEY_A, true);
	ui_down.add_key(KEY_S, true);
	ui_right.add_key(KEY_D, true);

	ui_up.add_controller_axis(0, ne::controller_axis::left_y, -27000);
	ui_left.add_controller_axis(0, ne::controller_axis::left_x, -27000);
	ui_down.add_controller_axis(0, ne::controller_axis::left_y, 27000);
	ui_right.add_controller_axis(0, ne::controller_axis::left_x, 27000);

	up.add_key(KEY_W);
	left.add_key(KEY_A);
	down.add_key(KEY_S);
	right.add_key(KEY_D);

	up.add_controller_axis(0, ne::controller_axis::left_y, -8000);
	left.add_controller_axis(0, ne::controller_axis::left_x, -8000);
	down.add_controller_axis(0, ne::controller_axis::left_y, 8000);
	right.add_controller_axis(0, ne::controller_axis::left_x, 8000);

	up_hit.add_key(KEY_W, true);
	left_hit.add_key(KEY_A, true);
	down_hit.add_key(KEY_S, true);
	right_hit.add_key(KEY_D, true);

	up_hit.add_controller_axis(0, ne::controller_axis::left_y, -8000);
	left_hit.add_controller_axis(0, ne::controller_axis::left_x, -8000);
	down_hit.add_controller_axis(0, ne::controller_axis::left_y, 8000);
	right_hit.add_controller_axis(0, ne::controller_axis::left_x, 8000);
}
