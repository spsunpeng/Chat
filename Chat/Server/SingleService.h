#pragma once

#include "CommonServiceBase.h"
#include "UserModelBase.h"
#include "FriendModelBase.h"
#include "GroupModelBase.h"
#include "groupUserModelBase.h"
#include<map>
#include<iostream>

std::unordered_map<string, muduo::net::TcpConnectionPtr> onlineUsers;
// 单机服务器业务实现
class SingleService : public CommonServiceBase
{
public:
	static SingleService& getInstance()
	{
		static SingleService instance;
		return instance;
	}
	SingleService(const SingleService&) = delete;
	SingleService& operator=(const SingleService&) = delete;

	// login service
	virtual void login(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time)
	{
		string name = js["username"];
		string pwd = js["userpwd"];

		UserDO user;
		user.setName(name);
		user.setPassword(pwd);
		user.setState("inline");

		if (userModelPtr->login(user)) // 登录成功
		{
			LOG_INFO << "login service!name->" << name << " pwd->"
				<< pwd << " success!!!";

			onlineUsers.insert(make_pair(name, con));

			json js;
			js["msgid"] = MSG_LOGIN_ACK;
			js["code"] = ACK_SUCCESS;
			// 登录成功，把用户的id返回
			js["userid"] = user.getID();
			std::cout << "userid" << user.getID() << std::endl;
			con->send(js.dump());
		}
		else // 登录失败
		{
			LOG_INFO << "login service!name->" << name << " pwd->"
				<< pwd << " error!!!";

			json js;
			js["msgid"] = MSG_LOGIN_ACK;
			js["code"] = ACK_ERROR;
			con->send(js.dump());
		}
	}

	// register service
	virtual void reg(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time)
	{
		string name = js["username"].get<string>();
		string pwd = js["userpwd"].get<string>();
		
		UserDO user;
		user.setID(-1);
		user.setName(name);
		user.setPassword(pwd);
		user.setState("offline");

		if (userModelPtr->add(user)) // 注册插入数据成功
		{
			LOG_INFO << "reg service!name->" << name << " pwd->"
				<< pwd << " success!!!";

			json js;
			js["msgid"] = MSG_REG_ACK;
			js["code"] = ACK_SUCCESS;
			con->send(js.dump());
		}
		else // 注册插入数据失败
		{
			LOG_INFO << "reg service!name->" << name << " pwd->"
				<< pwd << " error!!!";

			json js;
			js["msgid"] = MSG_REG_ACK;
			js["code"] = ACK_ERROR;
			con->send(js.dump());
		}
	}

	// add friend service
	virtual void addFriend(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time)
	{
		int userid= js["userid"];
		string friendName = js["friendName"];

		if (friendModelPtr->add(userid, friendName)) // 注册插入数据成功
		{
			LOG_INFO << "addFriend service success!!!";

			json js;
			js["msgid"] = MSG_ADD_FRIEND_ACK;
			js["code"] = ACK_SUCCESS;
			con->send(js.dump());
		}
		else // 注册插入数据失败
		{
			LOG_INFO << "addFriend service error!!!";

			json js;
			js["msgid"] = MSG_ADD_FRIEND_ACK;
			js["code"] = ACK_ERROR;
			con->send(js.dump());
		}
	}
	
	// add group service
	virtual void addGroup(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time)
	{
		string groupName = js["groupName"];

		GroupDO group;
		group.setName(groupName);

		if (groupModelPtr->add(group))
		{
			LOG_INFO << "addGroup service!name->" << groupName << " success!!!";

			json js;
			js["msgid"] = MSG_ADD_GROUP_ACK;
			js["code"] = ACK_SUCCESS;
			con->send(js.dump());
		}
		else // 注册插入数据失败
		{
			LOG_INFO << "reg service!name->" << groupName << " error!!!";

			json js;
			js["msgid"] = MSG_ADD_GROUP_ACK;
			js["code"] = ACK_ERROR;
			con->send(js.dump());
		}
	}

	//add groupUser service
	virtual void addGroupUser(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time)
	{
		string groupName = js["groupName"];
		int userid = js["userid"];
		string grouprole = js["grouprole"];

		if (groupUserModelPtr->add(groupName, userid, grouprole)) // 注册插入数据成功
		{
			LOG_INFO << "addGroupUser service success!!!";

			json js;
			js["msgid"] = MSG_ADD_GROUPUSER_ACK;
			js["code"] = ACK_SUCCESS;
			con->send(js.dump());
		}
		else // 注册插入数据失败
		{
			LOG_INFO << "addGroupUser service error!!!";

			json js;
			js["msgid"] = MSG_ADD_GROUPUSER_ACK;
			js["code"] = ACK_ERROR;
			con->send(js.dump());
		}
	}

	// one to one chat service
	virtual void oneChat(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time)
	{
		std::cout << "oneChat" << std::endl;
		muduo::net::TcpConnectionPtr tocon = onlineUsers.find(js["toName"])->second;

		js["msgid"] = MSG_ONE_CHAT_ACK;
		tocon->send(js.dump());
	}

	// group chat service
	virtual void groupChat(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time)
	{
		js["msgid"] = MSG_GROUP_CHAT_ACK;

		for (auto it = onlineUsers.begin(); it != onlineUsers.end(); ++it)
		{
			it->second->send(js.dump());
		}
	}


	// get onlineUser File
	virtual void onlineUserFile(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time)
	{
		json sendjs;
		sendjs["msgid"] = MSG_ONLINEUSER_FILE_ACK;

		auto it = onlineUsers.begin();
		string onUsers = it->first;
		++it;
		for (; it!=onlineUsers.end(); ++it)
		{
			onUsers += ", ";
			onUsers += it->first;			
		}		
		sendjs["onUsers"] = onUsers;
		sendjs["onUsersNum"] = onlineUsers.size();

		con->send(sendjs.dump());
	}

	// get group File
	virtual void groupFile(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time)
	{

	}

	// get friend File
	virtual void friendFile(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time)
	{
		
	}

private:
	unique_ptr<UserModelBase> userModelPtr;
	unique_ptr<FriendModelBase> friendModelPtr;
	unique_ptr<GroupModelBase> groupModelPtr;
	unique_ptr<groupUserModelBase> groupUserModelPtr;

	SingleService()
		: userModelPtr(new UserModel())
		, friendModelPtr(new FriendModel())
		, groupModelPtr(new GroupModel())
		, groupUserModelPtr(new groupUserModel())
	{}
};

// 全局接口，返回SingleService服务的唯一实例
static SingleService& App()
{
	return SingleService::getInstance();
}