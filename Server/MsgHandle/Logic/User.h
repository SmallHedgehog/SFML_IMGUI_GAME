#pragma once

/*
�û������߼�����ȡ�û���ϸ��Ϣ��
*/

#include "AbstractLogic.h"
#include "../Transaction/UserInfo.h"
#include "../../MessageType.h"
#include <iostream>


class User : public AbstractLogic
{
public:
	// �߼���������õ��û���Ϣ��
	void handleMessage(Socket* _clientSocket, const char* _msg, int size,
		MessageType _msgType = MessageType::TYPE_NONE);

private:
	void handleMessage_UserInfo(Socket* _clientSocket, const char* _msg, int size);
};
