#include "PlayerInfo.h"

const std::string PlayerInfo::get_death_message() {
	return death_message;
}

void PlayerInfo::set_death_message(const std::string &death_message) {
	this->death_message = death_message;
}

const std::string PlayerInfo::get_uuid() {
	return death_message;
}

void PlayerInfo::set_uuid(const std::string &uuid) {
	this->uuid = uuid;
}
