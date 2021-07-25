#include <string>

class PlayerNote {
private:
	int x;
	int y;

	static int counter;
	int id;

	std::string message;
	void set_message(const std::string &message);
public:
	PlayerNote(int x, int y, const std::string &message);
	const std::string get_message();

	int get_id();

	int get_x();
	int get_y();
};
