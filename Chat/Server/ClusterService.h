#pragma once

#include "CommonServiceBase.h"
#include "UserModelBase.h"
#include "FriendModelBase.h"
#include "GroupModelBase.h"
#include "groupUserModelBase.h"

#include "RedisServer.h"
#include "muduo/base/ThreadPool.h"

#include<map>
#include<iostream>

std::unordered_map<string, muduo::net::TcpConnectionPtr> onlineUsers;
std::unordered_map<string, int> nameAndId;
// 单机服务器业务实现
class ClusterService : public CommonServiceBase
{
public:
	static ClusterService& getInstance()
	{
		static ClusterService instance;
		return instance;
	}
	ClusterService(const ClusterService&) = delete;
	ClusterService& operator=(const ClusterService&) = delete;

	
	void handleChannelMsg(std::string msg)
	{
		LOG_INFO << "handleChannelMsg ";
		json js = json::parse(msg);
		string toName = js["toName"];
		auto it = onlineUsers.find(toName);
		js["msgid"] = MSG_ONE_CHAT_ACK;
		it->second->send(js.dump());
		LOG_INFO << "subscribe ";
	}


	// login service
	virtual void login(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time)
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
			int id = user.getID();
			std::cout << "name: "<< name <<"userid" << id << std::endl;

			onlineUsers.insert(make_pair(name, con));
			nameAndId.insert(make_pair(name, id));
			_redis.subscribe(id);

			json js;
			js["msgid"] = MSG_LOGIN_ACK;
			js["code"] = ACK_SUCCESS;
			// 登录成功，把用户的id返回
			js["userid"] = id;
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

	// one to one chat service
	virtual void oneChat(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time)
	{
		std::cout << "oneChat" << std::endl;
		string toName = js["toName"];
		std::cout << toName << std::endl;
		for (auto val : onlineUsers)
		{
			if (val.first == toName)
			{
				std::cout << val.first << std::endl;
				muduo::net::TcpConnectionPtr tocon = onlineUsers.find(toName)->second;

				js["msgid"] = MSG_ONE_CHAT_ACK;
				tocon->send(js.dump());

				return;
			}
		}
		std::cout << "publish stat"  << std::endl;
		int id = 8;
		std::cout << "id:" << id << std::endl;
		_redis.publish(id, js.dump());
		std::cout << "publish end" << std::endl;
	}

	// register service
	virtual void reg(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time)
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
	virtual void addFriend(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time)
	{
		int userid = js["userid"];
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
	virtual void addGroup(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time)
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
/*
	void clientCloseException(const muduo::net::TcpConnectionPtr& con, json& js, muduo::Timestamp time)
	{
		
	}*/


	// group chat service
	virtual void groupChat(const muduo::net::TcpConnectionPtr& con,
		json& js, muduo::Timestamp time)
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
		for (; it != onlineUsers.end(); ++it)
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

	RedisServer _redis;
	muduo::ThreadPool _pool;

	ClusterService()
	: userModelPtr(new UserModel())
	, friendModelPtr(new FriendModel())
	, groupModelPtr(new GroupModel())
	, groupUserModelPtr(new groupUserModel())
	{
		// 在RedisServer中注册通道消息回调
		if (_redis.connect())
		{
			// 给redis设置底层上报订阅消息的回调接口
			_redis.setChannelMsgHandler(boost::bind(&ClusterService::handleChannelMsg, this, _1));
			// 在单独的线程做监听通道信息，因为该操作是阻塞的
			_pool.start(1);
			_pool.run(boost::bind(&RedisServer::notifyMsg, &_redis));
		}
	}
};

// 全局接口，返回SingleService服务的唯一实例
static ClusterService& App()
{
	return ClusterService::getInstance();
}


/*
#include<map>
class Text
{
public:
	void Add(int id, string addr)
	{
		m[id] = addr;
	}

	void dispatch(int id, json js)
	{
		string addr = m[id];
		send(addr, js);
	}
private:
	std::map<int, string> m;
};

//加入：先连接connect
*/
