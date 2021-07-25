#include "GameSocket.h"
#include "PlayerInfo.h"
#include "PlayerNote.h"
#include "Utils.h"

std::unordered_set<WebSocketConnectionPtr> players;
std::mutex players_mutex;

std::unordered_set<std::shared_ptr<PlayerNote>> player_notes;
std::mutex notes_mutex;

Json::CharReader *reader = Json::CharReaderBuilder().newCharReader();

Json::Value note_jsonify(PlayerNote &note) {
	Json::Value coords;
	coords["x"] = note.get_x();
	coords["y"] = note.get_y();

	Json::Value message;
	message["type"] = "note";
	message["pos"] = coords;
	message["msg"] = note.get_message();

	return message;
}

void GameSocket::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type) {
	// ignore heartbeat and binary messages
	if (type != WebSocketMessageType::Text) return;

	Json::Value m;
	const char *c_str = message.c_str();
	JSONCPP_STRING err;
	bool success = reader->parse(
		c_str,
		c_str + message.length(),
		&m,
		&err
	);

	if (!success) {
		Json::Value error;
		error["type"] = "error";
		error["msg"] = err;
		wsConnPtr->send(err);
		return;
	}

	if (m["type"] == "pos") {
		auto sender_info = wsConnPtr->getContext<PlayerInfo>();
		players_mutex.lock();

		for (auto player : players) {
			if (wsConnPtr != player) {
				Json::Value message;
				message["type"] = "pos";
				message["uuid"] = sender_info->get_uuid();
				message["pos"] = m["pos"];
				player->send(stringify(message));
			}
		}

		players_mutex.unlock();
	} else if (m["type"] == "note") {
		PlayerNote note(m["x"].asInt(), m["y"].asInt(), m["msg"].asString());

		notes_mutex.lock();
		player_notes.insert(std::make_shared<PlayerNote>(note));
		notes_mutex.unlock();

		Json::Value message = note_jsonify(note);

		players_mutex.lock();
		for (auto player : players) {
			player->send(stringify(message));
		}
		players_mutex.unlock();
	} else {
		Json::Value error;
		error["type"] = "error";
		error["msg"] = "no such message type";
		wsConnPtr->send(stringify(error));
	}
}

void GameSocket::handleNewConnection(const HttpRequestPtr &req,const WebSocketConnectionPtr& wsConnPtr) {
	auto info = std::make_shared<PlayerInfo>();
	wsConnPtr->setContext(info);

	players_mutex.lock();
	players.insert(wsConnPtr);
	players_mutex.unlock();

	notes_mutex.lock();
	for (auto note : player_notes) {
		Json::Value msg = note_jsonify(*note);
		wsConnPtr->send(stringify(msg));
	}
	notes_mutex.unlock();
}

void GameSocket::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr) {
	std::cout << "someone left smh" << std::endl;

	Json::Value message;
	message["type"] = "leave";
	message["uuid"] = wsConnPtr->getContext<PlayerInfo>()->get_uuid();
	std::string leave_msg = stringify(message);

	players_mutex.lock();

	players.erase(wsConnPtr);
	for (auto player : players) {
		player->send(leave_msg);
	}

	players_mutex.unlock();
}
