#include "Login.h"


void Login::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	switch (_msgType)
	{
	case TYPE_SIGNIN:
		// ��¼�߼�����
		handleMessage_signin(_clientSocket, _msg, size);
		break;
	case TYPE_SIGNUP:
		// ע���߼�����
		handleMessage_signup(_clientSocket, _msg, size);
		break;
	case TYPE_SIGNOUT:
		// �ǳ��߼�����
		handleMessage_signout(_clientSocket, _msg, size);
		break;
	default:
		break;
	}
}

void Login::handleMessage_signout(Socket* _clientSocket, const char* _msg, int Size)
{
	std::cout << "�ǳ���Ϣ���ʹ���" << std::endl;

	if (_clientSocket)
	{
		// ������Ϣ
		std::vector<std::string> msgs;
		parserMessage(msgs, _msg);

		if (msgs.empty())
		{
			return;
		}
		else
		{
			if (msgs[0] == "NONE")
			{
				bool isFind = false;
				std::unordered_map<std::string, Socket*>::iterator it;
				for (it = Cache::cMaps.begin(); it != Cache::cMaps.end(); )
				{
					Socket* cSocket = it->second;
					if (&(*(cSocket)) == &(*(_clientSocket)))
					{
						msgs[0] = it->first;
						isFind = true;
						it = Cache::cMaps.erase(it);
						break;
					}
					else
						++it;
				}
				if (it == Cache::cMaps.end() && !isFind)
				{
					return;
				}
			}
		}
		// �����û�������Ϣ
		UserLogin::updateUserOnlineRecord(msgs[0], 0);	// 0 stand for the user isn't online
		// ֪ͨ���������û�
		DATA_PACKAGE notify_data;
		notify_data.dataHeader.dataSize = sizeof(notify_data) - sizeof(DATA_HEADER);
		notify_data.dataHeader.msgType = MessageType::TYPE_SIGNOUT;
		strcpy(notify_data.data, msgs[0].c_str());
		MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, notify_data);
	}
}

void Login::handleMessage_signin(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "��¼��Ϣ���ʹ���" << std::endl;

	DATA_PACKAGE dPackage;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);
	dPackage.dataHeader.msgType = MessageType::TYPE_SIGNIN;

	// ������Ϣ
	std::vector<std::string> msgs;
	parserMessage(msgs, _msg);
	if (msgs.size() != 2)
	{
		strcpy(dPackage.data, "PARAMETOR_Signin message parameter error!");
	}
	else
	{
		// �����ݿ��в�ѯ�Ƿ��и��û�
		std::string passwd;
		if (UserLogin::userIsExistsAndGetPasswd(msgs[0], passwd))
		{
			// �û���ѯ�ɹ�����Ҫ��֤�û�����
			if (passwd == msgs[1])
			{
				std::cout << "�û���¼����֤�û���������ɹ�" << std::endl;
				strcpy(dPackage.data, "SUCCESS_NONE");
				
				// �����û�������Ϣ
				UserLogin::updateUserOnlineRecord(msgs[0], 1);	// 1 stand for the user is online
				
				// ����username->cSocket
				Cache::cMaps[msgs[0]] = _clientSocket;

				// ֪ͨ���������û�
				DATA_PACKAGE notify_data;
				notify_data.dataHeader.dataSize = sizeof(notify_data) - sizeof(DATA_HEADER);
				notify_data.dataHeader.msgType = MessageType::TYPE_SIGNIN;
				std::string sendData = msgs[0] + "_NONE";
				strcpy(notify_data.data, sendData.c_str());
				MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, notify_data);
			}
			else
			{
				std::cout << "�û���¼����֤����ʧ��" << std::endl;
				strcpy(dPackage.data, "PASSWORD_User's password error!");
			}
		}
		else
		{
			std::cout << "�û���¼�������û���ʧ��" << std::endl;
			strcpy(dPackage.data, "USERNAME_User's username error!");
		}
	}

	if (_clientSocket)
		_clientSocket->write((char*)&dPackage, sizeof(dPackage));
}

void Login::handleMessage_signup(Socket* _clientSocket, const char* _mgs, int size)
{
	// ע����Ϣ���ʹ���
	std::cout << "ע����Ϣ���ʹ���" << std::endl;
	
	/* ��ѯ���ݿ����Ƿ����и��û�
	   ���û�и��û����������ݿ��в���һ���û���Ϣ
	   ��ע�������ע��ɹ���ʧ�ܵ���Ϣ
	*/
	// �������͹�������Ϣ

	DATA_PACKAGE dPackage;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);
	dPackage.dataHeader.msgType = MessageType::TYPE_SIGNUP;

	std::vector<std::string> msgs;
	parserMessage(msgs, _mgs);
	if (msgs.size() != 2)
	{
		strcpy(dPackage.data, "PARAMETOR_SIGNUP MESSAGE PARAMETER ERROR!");
	}
	else
	{
		// �����ݿ��в�ѯ�Ƿ��и��û�
		bool isFind = UserLogin::userIsExists(msgs[0]);
		if (isFind)
		{
			std::cout << "EXISTS" << std::endl;
			// ��ע�������ע��ʧ����Ϣ
			strcpy(dPackage.data, "EXISTS_THE USER HAVE EXSIT!");
		}
		else
		{
			if (UserLogin::insertUserRecord(msgs[0], msgs[1]))
			{
				std::cout << "SUCCESS" << std::endl;
				// ��ע�������ע��ɹ���Ϣ
				strcpy(dPackage.data, "SUCCESS_NONE");
				// ���û�������Ϣ���в���һ���û�������Ϣ
				UserLogin::insertUserOnlineRecord(msgs[0]);
				// ���û���Ϣ���в���һ����¼
				UserInfo::setUserInfo(msgs[0]);
			}
			else
			{
				std::cout << "FAILD" << std::endl;
				// ��ע�������ע��ʧ����Ϣ
				strcpy(dPackage.data, "UNKNOWN_INSERT USER'S DATA ERROR!");
			}
		}
	}

	if (_clientSocket)
		_clientSocket->write((char*)&dPackage, sizeof(dPackage));
}
