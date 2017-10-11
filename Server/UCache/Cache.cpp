#include "Cache.h"

std::list<Socket*> Cache::cSockets;
std::list<virtualThread*> Cache::cThreads;

// ����cSocket->username
std::unordered_map<std::string, Socket*> Cache::cMaps;

// ����ƥ��ɹ���username->username
std::unordered_map<std::string, std::string> Cache::cUsers;
