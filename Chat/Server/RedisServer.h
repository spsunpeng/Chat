#pragma once

#include <hiredis/hiredis.h>
#include <string>
#include <muduo/base/Logging.h>

// 配置redis server的网络地址信息
static std::string redisHost = "127.0.0.1";
static unsigned short port = 6379;

class RedisServer
{
public:
	// 初始化连接redis server的context上下文环境
	RedisServer() 
		:_context(nullptr)
		,_pubcontext(nullptr)
	{}
	// 释放和redis server连接用的context上下文环境
	~RedisServer()
	{
		if (this->_context != nullptr)
		{
			redisFree(this->_context);
			this->_context = nullptr;
		}
		if (this->_pubcontext != nullptr)
		{
			redisFree(this->_pubcontext);
			this->_pubcontext = nullptr;
		}
	}
	// 连接redis server
	bool connect()
	{
		this->_context = redisConnect(redisHost.c_str(), port);
		this->_pubcontext = redisConnect(redisHost.c_str(), port);
		if (this->_context == nullptr || this->_context->err)
		{
			LOG_ERROR << "redis server connect error! reason:"
				<< this->_context->errstr;
			return false;
		}
		LOG_INFO << "redis server connect success!";
		return true;
	}
	// 订阅通道
	void subscribe(int channel)
	{
		if (REDIS_ERR == redisAppendCommand(this->_context, "SUBSCRIBE %d", channel))
		{
			LOG_ERROR << "subscribe [" << channel << "] error1!";
			return;
		}
		int done = 0;
		while (!done)
		{
			if (REDIS_ERR == redisBufferWrite(this->_context, &done))
			{
				LOG_ERROR << "subscribe [" << channel << "] error2!";
				return;
			}
		}
		LOG_INFO << "subscribe [" << channel << "] success!";
	}

	//发布
	void publish(int channel, std::string msg)
	{
		std::cout << "publish in" << std::endl;
		redisReply* reply = (redisReply*)redisCommand(this->_pubcontext, "PUBLISH %d %s", channel, msg.c_str());
		if (reply == nullptr)
		{
			LOG_ERROR << "publish [" << channel << "] error!";
			return;
		}
		freeReplyObject(reply);
		LOG_INFO << "publish [" << channel << "] success!";
	}

	// 取消订阅的通道
	void unsubscribe(int channel)
	{
		redisReply *reply = (redisReply*)redisCommand(this->_context, "UNSUBSCRIBE %d", channel);
		if (reply == nullptr)
		{
			LOG_ERROR << "subscribe [" << channel << "] error!";
			return;
		}
		freeReplyObject(reply);
		LOG_INFO << "unsubscribe [" << channel << "] success!";
	}

	// 接收/检测：检测通道消息，必须放在单独的线程中处理，通道没有消息，redisGetReply会阻塞线程
	void notifyMsg()
	{
		redisReply *reply;
		while (redisGetReply(this->_context, (void**)&reply) == REDIS_OK)
		{
			if (reply != nullptr && reply->element != nullptr)
			{
				if (reply->element[2] != nullptr
					 && reply->element[2]->str != nullptr)
				{
					LOG_INFO << reply->element[2]->str;
					_channelHandler(reply->element[2]->str);
				}
			}
			else
			{
				LOG_INFO << "redisGetReply no response!";
			}
		}

		freeReplyObject(reply);
	}
	
	// 设置通道消息上报的回调函数，当RedisServer检测有订阅的消息发生，则通过预先设置的handler上报服务层
	using channelHandler = std::function<void(std::string)>;
	void setChannelMsgHandler(channelHandler handler)
	{
		_channelHandler = handler;
	}
private:
	redisContext* _context;
	redisContext* _pubcontext;
	channelHandler _channelHandler;
};

/*
virtual void oneChat(const muduo::net::TcpConnectionPtr &con,
		json &js, muduo::Timestamp time)
	{
		int toid = js["toid"];

		// 查询toid用户是否在线
		std::string state = userModelPtr->useState(toid);
		if (state == "offline") // 用户不在线
		{
			// toid用户不在线，给fromid用户回复
			json j;
			j["msgid"] = MSG_ONE_CHAT_ACK;
			j["code"] = ACK_ERROR;
			con->send(j.dump());

			// 存储离线信息
			offlineModelPtr->add(toid, js.dump()); // 存储离线消息
		}
		else // 用户在线
		{
			auto it = _userConnMap.find(toid);
			if (it != _userConnMap.end())
			{
				// toid用户在同一服务器上，可以直接转发信息
				it->second->send(js.dump());
			}
			else
			{
				// toid用户在集群环境其它服务器上，向中间件消息队列发布消息
				_redisServer.publish(toid, js.dump());
			}
		}
	}

*/