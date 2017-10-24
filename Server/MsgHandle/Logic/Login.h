#pragma once

/*
�����¼��ע����Ϣ����
*/

#include "AbstractLogic.h"
#include "../Transaction/UserLogin.h"
#include "../Transaction/UserInfo.h"
#include "../../MessageType.h"
#include <iostream>


class Login : public AbstractLogic
{
public:
	// �߼����������û���¼
	void handleMessage(Socket* _clientSocket, const char* _msg, int size,
		MessageType _msgType = MessageType::TYPE_NONE);

private:
	// �߼����������û�ע��
	void handleMessage_signup(Socket* _clientSocket, const char* _msg, int Size);
	
	// �߼����������û���¼
	void handleMessage_signin(Socket* _clientSocket, const char* _msg, int Size);

	// �߼����������û��ǳ�
	void handleMessage_signout(Socket* _clientSocket, const char* _msg, int Size);
};
