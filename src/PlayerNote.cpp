#include "PlayerNote.h"

int PlayerNote::counter;

PlayerNote::PlayerNote(int x, int y, const std::string &message) {
	this->x = x;
	this->y = y;

	id = this->counter++;

	set_message(message);
}

void PlayerNote::set_message(const std::string &message) {
	this->message = message;
}

const std::string PlayerNote::get_message() {
	return message;
}

int PlayerNote::get_id() {
	return id;
}

int PlayerNote::get_x() {
	return x;
}

int PlayerNote::get_y() {
	return y;
}
