#include "PlayerInfo.h"
#include <drogon/utils/Utilities.h>

PlayerInfo::PlayerInfo() {
	set_uuid(drogon::utils::getUuid());
}

const std::string PlayerInfo::get_uuid() {
	return uuid;
}

void PlayerInfo::set_uuid(const std::string &uuid) {
	this->uuid = uuid;
}
