#include "Fight.h"


void Fight::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	handleMessage_fight(_clientSocket, _msg, size);
}

void Fight::handleMessage_fight(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "�û���ս��Ϣ����" << std::endl;

	// ������Ϣ����
	std::vector<std::string> msgs;
	parserMessage(msgs, _msg);

	if (msgs.size() != 5)
	{
		if (msgs.size() == 7)	// ���������˳�������
		{
			// ��ս������Ϣ���û���+�û���Ϣ
			std::vector<std::string> userInfos;
			userInfos.assign(msgs.begin() + 1, msgs.end() - 1);
			UserInfo::setUserInfo(msgs[0], userInfos);

			// ����username->username
			if (Cache::cUsers.find(msgs[0]) != Cache::cUsers.end())
				Cache::cUsers.erase(msgs[0]);

			DATA_PACKAGE data;
			data.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
			data.dataHeader.msgType = MessageType::TYPE_FIGHT;
			std::string sendData = msgs[0] + "_END";
			strcpy(data.data, sendData.c_str());
			MultiMessageSend::messageSend(Cache::cSockets, data);
		}
		else if (msgs.size() == 8)	// ����
		{
			// ��ս������Ϣ���û���+�û���Ϣ
			std::vector<std::string> userInfos;
			userInfos.assign(msgs.begin() + 2, msgs.end() - 1);
			UserInfo::setUserInfo(msgs[0], userInfos);

			// ����username->username
			if (Cache::cUsers.find(msgs[0]) != Cache::cUsers.end())
				Cache::cUsers.erase(msgs[0]);

			// ���ұ����ͷ���Socket
			Socket* respSocket = nullptr;
			std::unordered_map<std::string, Socket*>::iterator it = Cache::cMaps.begin();
			for (; it != Cache::cMaps.end(); ++it)
			{
				if (it->first == msgs[1])
				{
					respSocket = it->second;
					break;
				}
			}
			if (it != Cache::cMaps.end())
			{
				// �򱻷��ͷ�д����Ϣ
				DATA_PACKAGE dPackage;
				dPackage.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
				dPackage.dataHeader.msgType = MessageType::TYPE_FIGHT;
				std::string send_data = msgs[0] + "_RUN";
				strcpy(dPackage.data, send_data.c_str());
				if (respSocket)
					respSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
			}

			DATA_PACKAGE data;
			data.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
			data.dataHeader.msgType = MessageType::TYPE_FIGHT;
			std::string sendData = msgs[0] + "_END";
			strcpy(data.data, sendData.c_str());
			MultiMessageSend::messageSend(Cache::cSockets, data);
		}
	}
	else
	{
		// ���ұ����ͷ���Socket
		Socket* respSocket = nullptr;
		std::unordered_map<std::string, Socket*>::iterator it = Cache::cMaps.begin();
		for (; it != Cache::cMaps.end(); ++it)
		{
			if (it->first == msgs[1])
			{
				respSocket = it->second;
				break;
			}
		}
		if (it != Cache::cMaps.end())
		{
			// �򱻷��ͷ�д����Ϣ
			DATA_PACKAGE dPackage;
			dPackage.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
			dPackage.dataHeader.msgType = MessageType::TYPE_FIGHT;
			strcpy(dPackage.data, _msg);
			if (respSocket)
				respSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
		}
		else
		{
			// ����ʧ�ܣ����ͷ�д��ʧ����Ϣ
			DATA_PACKAGE data;
			data.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
			data.dataHeader.msgType = MessageType::TYPE_FIGHT;
			std::string sendData = msgs[0] + "_" + msgs[1] + "_FAIL";
			strcpy(data.data, sendData.c_str());
			if (_clientSocket)
				_clientSocket->write((char*)&data, sizeof(DATA_PACKAGE));
		}
	}
}
