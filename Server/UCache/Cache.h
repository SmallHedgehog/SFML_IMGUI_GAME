#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include "../Network/Socket.h"
#include "../Threads/virtualThread.h"


class Cache
{
public:
	static std::list<Socket*> cSockets;
	static std::list<virtualThread*> cThreads;

	// ����username->cSocket
	static std::unordered_map<std::string, Socket*> cMaps;

	// ����ƥ��ɹ���username->username
	static std::unordered_map<std::string, std::string> cUsers;
};
