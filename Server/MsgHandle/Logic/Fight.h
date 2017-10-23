#pragma once

/*
�û���ս�߼�����
*/

#include "AbstractLogic.h"
#include "../../MessageType.h"
#include "../Transaction/UserInfo.h"
#include <iostream>


class Fight : public AbstractLogic
{
public:
	// �߼������û���ս
	void handleMessage(Socket* _clientSocket, const char* _msg, int size,
		MessageType _msgType = MessageType::TYPE_NONE);

private:
	void handleMessage_fight(Socket* _clientSocket, const char* _msg, int size);
};
