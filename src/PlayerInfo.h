#include <string>

class PlayerInfo {
private:
	std::string uuid;
	void set_uuid(const std::string &uuid);
public:
	PlayerInfo();
	const std::string get_uuid();
};
