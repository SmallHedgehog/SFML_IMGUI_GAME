#pragma once

/*
�û�ƥ���߼�����
*/

#include "AbstractLogic.h"
#include "../../MessageType.h"
#include <iostream>


class Match : public AbstractLogic
{
public:
	// �߼������û�ƥ�䴦��
	void handleMessage(Socket* _clientSocket, const char* _msg, int size,
		MessageType _msgType = MessageType::TYPE_NONE);

private:
	void handleMessage_matchUser(Socket* _clientSocket, const char* _msg, int size);
};
