#include <string>

class PlayerInfo {
private:
	std::string death_message = "BRUH";

	// we might not use but it's probably good to add it just in case
	std::string uuid = "";
public:
	const std::string get_death_message();
	void set_death_message(const std::string &death_message);

	const std::string get_uuid();
	void set_uuid(const std::string &uuid);
};
