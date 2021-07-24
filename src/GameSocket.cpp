#include "GameSocket.h"
#include "PlayerInfo.h"
#include "Utils.h"

std::unordered_set<WebSocketConnectionPtr> players;
std::mutex players_mutex;

Json::CharReader *reader = Json::CharReaderBuilder().newCharReader();

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

	if (m["type"] == "relay") {
		players_mutex.lock();

		for (auto player : players) {
			if (wsConnPtr != player) {
				player->send(m["msg"].asString());
			}
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
	info->set_uuid(drogon::utils::getUuid());
	wsConnPtr->setContext(info);

	players_mutex.lock();
	players.insert(wsConnPtr);
	players_mutex.unlock();
}

void GameSocket::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr) {
	std::cout << "someone left smh" << std::endl;

	players_mutex.lock();
	players.erase(wsConnPtr);
	players_mutex.unlock();
}
