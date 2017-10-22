#include "Match.h"


void Match::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	handleMessage_matchUser(_clientSocket, _msg, size);
}

void Match::handleMessage_matchUser(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "�û�ƥ����Ϣ����" << std::endl;

	// ������Ϣ����
	std::vector<std::string> msgs;
	parserMessage(msgs, _msg);

	int msgsSize = msgs.size();
	if (msgsSize == 2)
	{
		bool sign = false;
		if (Cache::cUsers.find(msgs[0]) != Cache::cUsers.end() ||
			Cache::cUsers.find(msgs[1]) != Cache::cUsers.end())
		{
			sign = true;
		}
		else
		{
			// �û�����ƥ����Ϣ
			Socket* respSocket = nullptr;
			std::unordered_map<std::string, Socket*>::iterator it = Cache::cMaps.begin();
			for (; it != Cache::cMaps.end(); ++it)
			{
				// ���ҵ���ƥ���û���Socket
				if (it->first == msgs[1])
				{
					respSocket = it->second;
					break;
				}
			}
			if (it == Cache::cMaps.end())
			{
				// û���ҵ���ƥ���û���Socket
				sign = true;
			}
			else
			{
				// �������û�����ƥ����Ϣ
				if (respSocket)
				{
					DATA_PACKAGE dPackage;
					dPackage.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
					dPackage.dataHeader.msgType = MessageType::TYPE_MATCH;
					std::string sendData = msgs[1] + "_" + msgs[0];
					strcpy(dPackage.data, sendData.c_str());

					respSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
				}
			}
		}
		if (sign)
		{
			DATA_PACKAGE data;
			data.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
			data.dataHeader.msgType = MessageType::TYPE_MATCH;
			std::string send_data = msgs[0] + "_" + msgs[1] + "_FAIL";
			strcpy(data.data, send_data.c_str());
			if (_clientSocket)
				_clientSocket->write((char*)&data, sizeof(DATA_PACKAGE));
		}
	}
	else if (msgsSize == 3)
	{
		// �������Ա������û���ƥ����Ϣ
		Socket* respSocket = nullptr;
		bool sign = false;
		std::unordered_map<std::string, Socket*>::iterator it = Cache::cMaps.begin();
		for (; it != Cache::cMaps.end(); ++it)
		{
			if (it->first == msgs[1])
			{
				respSocket = it->second;
				break;
			}
		}
		if (it == Cache::cMaps.end())
		{
			sign = true;
		}
		else
		{
			if (respSocket)
			{
				DATA_PACKAGE dPackage;
				dPackage.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
				dPackage.dataHeader.msgType = MessageType::TYPE_MATCH;
				std::string sendData = msgs[1] + "_" + msgs[0] + "_" + msgs[2];
				strcpy(dPackage.data, sendData.c_str());
				if (msgs[2] == "YES")
				{
					// �����û����û���ƥ����Ϣ����ս������ʱ��Ҫɾ��
					Cache::cUsers[msgs[0]] = msgs[1];
					Cache::cUsers[msgs[1]] = msgs[0];
					
					// ֪ͨ�����û�
					DATA_PACKAGE Package;
					Package.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
					Package.dataHeader.msgType = MessageType::TYPE_MATCH;
					std::string sendData = msgs[0] + "_" + msgs[1] + "_MATCHED";
					strcpy(Package.data, sendData.c_str());
					MultiMessageSend::messageSend(Cache::cSockets, Package);
				}
				
				respSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
			}
			else
			{
				sign = true;
			}
		}
		if (sign)
		{
			DATA_PACKAGE data;
			data.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
			data.dataHeader.msgType = MessageType::TYPE_MATCH;
			std::string send_data = msgs[0] + "_" + msgs[1] + "_FAIL";
			strcpy(data.data, send_data.c_str());
			if (_clientSocket)
				_clientSocket->write((char*)&data, sizeof(DATA_PACKAGE));
		}
	}
	else
	{
		DATA_PACKAGE data;
		data.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
		data.dataHeader.msgType = MessageType::TYPE_MATCH;
		std::string send_data = "ERROR";
		strcpy(data.data, send_data.c_str());
		if (_clientSocket)
			_clientSocket->write((char*)&data, sizeof(DATA_PACKAGE));
	}
}
