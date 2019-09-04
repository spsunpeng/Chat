#pragma once

#include <hiredis/hiredis.h>
#include <string>
#include <muduo/base/Logging.h>

// ����redis server�������ַ��Ϣ
static std::string redisHost = "127.0.0.1";
static unsigned short port = 6379;

class RedisServer
{
public:
	// ��ʼ������redis server��context�����Ļ���
	RedisServer() 
		:_context(nullptr)
		,_pubcontext(nullptr)
	{}
	// �ͷź�redis server�����õ�context�����Ļ���
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
	// ����redis server
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
	// ����ͨ��
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

	//����
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

	// ȡ�����ĵ�ͨ��
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

	// ����/��⣺���ͨ����Ϣ��������ڵ������߳��д���ͨ��û����Ϣ��redisGetReply�������߳�
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
	
	// ����ͨ����Ϣ�ϱ��Ļص���������RedisServer����ж��ĵ���Ϣ��������ͨ��Ԥ�����õ�handler�ϱ������
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

		// ��ѯtoid�û��Ƿ�����
		std::string state = userModelPtr->useState(toid);
		if (state == "offline") // �û�������
		{
			// toid�û������ߣ���fromid�û��ظ�
			json j;
			j["msgid"] = MSG_ONE_CHAT_ACK;
			j["code"] = ACK_ERROR;
			con->send(j.dump());

			// �洢������Ϣ
			offlineModelPtr->add(toid, js.dump()); // �洢������Ϣ
		}
		else // �û�����
		{
			auto it = _userConnMap.find(toid);
			if (it != _userConnMap.end())
			{
				// toid�û���ͬһ�������ϣ�����ֱ��ת����Ϣ
				it->second->send(js.dump());
			}
			else
			{
				// toid�û��ڼ�Ⱥ���������������ϣ����м����Ϣ���з�����Ϣ
				_redisServer.publish(toid, js.dump());
			}
		}
	}

*/