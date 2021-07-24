#include <string>

class PlayerInfo {
private:
	std::string death_message = "BRUH";

	std::string uuid;
	void set_uuid(const std::string &uuid);
public:
	PlayerInfo();

	const std::string get_death_message();
	void set_death_message(const std::string &death_message);

	const std::string get_uuid();
};
