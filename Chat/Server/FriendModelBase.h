#pragma once

#include "MySQL.h"
#include<iostream>

// Model层的抽象类
class FriendModelBase
{
public:
	// 增加好友信息
	virtual bool add(int userid, string friendName) = 0;
};

// Friend表的Model层操作
class FriendModel : public FriendModelBase
{
public:
	bool add(int userid, string friendName)
	{
		std::cout << "FriendModel::add" << std::endl;
		//通过用户名获取id
		int friendid = getid(friendName);
				
		// 组织sql语句
		char sql[1024] = { 0 };
		sprintf(sql, "insert into friend(userid,friendid) values(%d, %d)", userid, friendid);

		MySQL mysql;
		if (mysql.connect())
		{
			if (mysql.update(sql))
			{
				LOG_INFO << "add friendid success => sql:" << sql;
				return true;
			}
		}
		LOG_INFO << "add friendid error => sql:" << sql;
		return false;
	}

	//通过用户名获取id
	int getid(string name)
	{
		std::cout << "FriendModel::getid" << std::endl;
		char sql[1024] = { 0 };
		sprintf(sql, "select id from user where name='%s'", name.c_str());

		int id = 0;
		MySQL mysql;		
		if (mysql.connect())
		{
			// 注意MYSQL_RES指针永远不为空，不管是否查询到数据
			MYSQL_RES* res = mysql.query(sql);
			if (res != nullptr)
			{
				MYSQL_ROW row = mysql_fetch_row(res);  //通过数据库获取id
				// MYSQL_ROW是char**，通过row是否为nullptr，判断查询是否返回结果
				if (row != nullptr)
				{
					LOG_INFO << "login success => sql:" << sql;
					id = atoi(row[0]);
					std::cout << "friendid" << id << std::endl;
					mysql_free_result(res);
				}
			}
		}
		return id;
	}
};
