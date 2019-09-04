#include "ChatClient.h"
#include <iostream>
#include "public.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;


// TcpClient�󶨻ص������������ӻ��߶Ͽ�������ʱ����
void ChatClient::onConnection(const muduo::net::TcpConnectionPtr &con)
{
	/*
	���ӷ������ɹ��󣬿����ͷ������Ľ���ͨ�Ź���
	*/
	if (con->connected()) // �ͷ��������ӳɹ�
	{
		LOG_INFO << "connect server success!";
		// �����߳�ר�Ŵ����û����������
		_pool.start(1);
		_pool.run(bind(&ChatClient::userClient, this, con));
	}
	else // �ͷ���������ʧ��
	{

	}
}

int myid = 0;
string myname;
volatile bool isLoginSuccess = false;
volatile bool isRegSuccess = false;
volatile bool isAddFriendSuccess = false;
volatile bool isAddGroupSuccess = false;
volatile bool isOneChatSuccess = false;
volatile bool isGroupChatSuccess = false;
volatile bool isAddGroupUserSuccess = false;

// TcpClient�󶨻ص��������������ݽ���ʱ����
void ChatClient::onMessage(const muduo::net::TcpConnectionPtr &con,
	muduo::net::Buffer *buf,
	muduo::Timestamp time)
{
	/*
	������շ�������Ӧ����Ϣ���ڿͻ��˽�����ʾ
	*/
	muduo::string msg = buf->retrieveAllAsString();
	json js = json::parse(msg);

	switch (js["msgid"].get<int>())
	{
	case MSG_LOGINOUT:
		break;

	case MSG_LOGIN_ACK:		
		isLoginSuccess = (js["code"].get<int>() == ACK_SUCCESS ? true : false);
		if (isLoginSuccess)
		{
			myid = js["userid"].get<int>();
		}
		sem_post(&_loginSem);
		break;

	case MSG_REG_ACK:
		isRegSuccess = (js["code"].get<int>() == ACK_SUCCESS ? true : false);
		sem_post(&_RegSem);
		break;

	case MSG_ADD_FRIEND_ACK:
		isAddFriendSuccess = (js["code"].get<int>() == ACK_SUCCESS ? true : false);
		sem_post(&_addFriendSem);
		break;

	case MSG_ADD_GROUP_ACK:
		isAddGroupSuccess = (js["code"].get<int>() == ACK_SUCCESS ? true : false);
		sem_post(&_addGroupSem);
		break;

	case MSG_ADD_GROUPUSER_ACK:
		isAddGroupUserSuccess = (js["code"].get<int>() == ACK_SUCCESS ? true : false);
		sem_post(&_addGroupUserSem);
		break;

	case MSG_ONE_CHAT_ACK:
		std::cout << js["fromName"].get<string>()<<": "<< js["chatData"].get<string>() << std::endl;
		break;

	case MSG_GROUP_CHAT_ACK:
		std::cout << js["fromName"].get<string>() << ": " << js["chatData"].get<string>() << std::endl;
		break;

	case MSG_ONLINEUSER_FILE_ACK:
		std::cout <<"OnlineUsers is "<<js["onUsersNum"]<<": "<< js["onUsers"].get<string>() << std::endl;
		sem_post(&_onUserSem);
		break;

	default:
		break;
	}	
}

// �����û����������
void ChatClient::userClient(const muduo::net::TcpConnectionPtr &con)
{
	int choice = 0;
	while (1)
	{
		std::cout << "============================================" << std::endl;
		std::cout << "0.Exit" << std::endl;
		std::cout << "1.Login" << std::endl;
		std::cout << "2.Register" << std::endl;
		std::cout << "============================================" << std::endl;
		std::cin >> choice;

		switch (choice)
		{
		case 0:
			break;//1.����������Ϣ�Ľ��̣�2.���������Ϣ�Ľ��̣���Ϣ����3.�Ͽ����ӣ�muduo��
		case 1:
			Login(con);
			break;
		case 2:
			Register(con);
			break;
		default:
			cout << "Please input 0-2!" << endl;
			break;
		}
		
		if (isLoginSuccess)
		{
			while (1)
			{
				std::cout << "============================================" << std::endl;
				std::cout << "0.Exit" << std::endl;
				std::cout << "1.oneChat" << std::endl;
				std::cout << "2.groupChat" << std::endl;
				std::cout << "3.addFriend" << std::endl;
				std::cout << "4.addGroup" << std::endl;
				std::cout << "5.addGroupUser" << std::endl;
				std::cout << "============================================" << std::endl;
				std::cin >> choice;

				bool exitFlag = false;

				switch (choice)
				{
				case 0:
					exitFlag = true;
					break;
				case 1:
					oneChat(con);
					break;
				case 2:
					groupChat(con);
					break;
				case 3:
					addFriend(con);
					break;
				case 4:
					addGroup(con);
					break;
				case 5:
					addGroupUser(con);
					break;
				default:
					cout << "Please input 0-5!" << endl;
					break;
				}

				if (exitFlag)
					break;
			}			
		}		
	}
}

void ChatClient::Login(const muduo::net::TcpConnectionPtr& con)
{
	while (1)
	{
		char name[50] = { 0 };
		char pwd[50] = { 0 };
		std::cout << "username: ";
		std::cin >> name;
		std::cout << "password: ";
		std::cin >> pwd;

		json js;
		js["msgid"] = MSG_LOGIN;
		js["username"] = name;
		js["userpwd"] = pwd;
		con->send(js.dump());//

		sem_wait(&_loginSem);
		
		if (isLoginSuccess)
		{
			myname = name;
			std::cout << "Login Success! Please continue" << std::endl;
			return;
		}			
		std::cout << "name or pwd is wrong! Please re-enter: " << std::endl;
	}	
}

void ChatClient::Register(const muduo::net::TcpConnectionPtr& con)
{
	while (1)
	{
		char name[50] = { 0 };
		char pwd[50] = { 0 };
		std::cout << "regname: ";
		std::cin >> name;
		std::cout << "regpassword: ";
		std::cin >> pwd;

		json js;
		js["msgid"] = MSG_REG;
		js["username"] = name;
		js["userpwd"] = pwd;
		con->send(js.dump());

		sem_wait(&_RegSem);

		if (isRegSuccess)
		{
			std::cout << "Register Success! Please continue" << std::endl;
			return;
		}
		std::cout << "name or pwd is wrong! Please re-enter: " << std::endl;
	}
}


void ChatClient::addFriend(const muduo::net::TcpConnectionPtr& con)
{
	while (1)
	{
		char friendName[50] = { 0 };
		std::cout << "friendName: ";
		std::cin >> friendName;

		json js;
		js["msgid"] = MSG_ADD_FRIEND;
		js["userid"] = myid;
		js["friendName"] = friendName;
		con->send(js.dump());

		sem_wait(&_addFriendSem);

		if (isAddFriendSuccess)
		{
			std::cout << "addFriend Success! Please continue" << std::endl;
			return;
		}
		std::cout << "friendName is wrong! Please re-enter: " << std::endl;
	}
	
}

void ChatClient::addGroup(const muduo::net::TcpConnectionPtr& con)
{
	while (1)
	{
		//����Ⱥ��
		char groupName[50] = { 0 };
		std::cout << "groupName: ";
		std::cin >> groupName;

		json js;
		js["msgid"] = MSG_ADD_GROUP;
		js["groupName"] = groupName;

		con->send(js.dump());

		sem_wait(&_addGroupSem);

		if (isAddGroupSuccess)
		{			
			std::cout << "AddGroup Success! Please continue" << std::endl;
			addGroupUser(con, true);//�Զ���ΪȺ��
			return;
		}
		std::cout << "groupName is wrong! Please re-enter: " << std::endl;
	}
}

void ChatClient::oneChat(const muduo::net::TcpConnectionPtr& con)
{
	//��ȡ�����û�
	json js;
	js["msgid"] = MSG_ONLINEUSER_FILE;	
	con->send(js.dump());

	sem_wait(&_onUserSem);
	
	string toName;
	std::cout << "please choice toName: ";
	std::cin >> toName;

	js["msgid"] = MSG_ONE_CHAT;
	js["fromName"] = myname;
	js["toName"] = toName;

	//����
	std::cout << "You chat with "<<toName<<" now! If you want to exit, please enter exitchat!" << std::endl;
	while (1)
	{
		string chatData;
		std::cin >> chatData;		

		if (chatData == "exitchat")
		{
			break;
		}
		
		js["chatData"] = chatData;
		con->send(js.dump());
	}
}

void ChatClient::groupChat(const muduo::net::TcpConnectionPtr& con)
{
	std::cout << "You chat with all online users now! If you want to exit, please enter exitchat!" << std::endl;

	json js;
	js["msgid"] = MSG_GROUP_CHAT;
	js["fromName"] = myname;

	while (1)
	{
		string chatData;
		std::cin >> chatData;

		if (chatData == "exitchat")
		{
			break;
		}

		js["chatData"] = chatData;
		con->send(js.dump());
	}
}

void ChatClient::addGroupUser(const muduo::net::TcpConnectionPtr& con, bool isCreate)
{
	while (1)
	{
		char groupName[50] = { 0 };
		std::cout << "groupName: ";
		std::cin >> groupName;

		json js;
		js["msgid"] = MSG_ADD_GROUPUSER;
		js["groupName"] = groupName;
		js["userid"] = myid;
		js["grouprole"] = "normal";
		if (isCreate)
		{
			js["grouprole"] = "creator";
		}

		con->send(js.dump());

		sem_wait(&_addGroupUserSem);

		if (isAddGroupUserSuccess==true)
		{
			std::cout << "addGroupUser Success! Please continue" << std::endl;
			return;
		}
		std::cout << "groupName is wrong! Please re-enter: " << std::endl;
	}
}