#pragma once

#include "MySQL.h"
#include<iostream>

// Model��ĳ�����
class FriendModelBase
{
public:
	// ���Ӻ�����Ϣ
	virtual bool add(int userid, string friendName) = 0;
};

// Friend���Model�����
class FriendModel : public FriendModelBase
{
public:
	bool add(int userid, string friendName)
	{
		std::cout << "FriendModel::add" << std::endl;
		//ͨ���û�����ȡid
		int friendid = getid(friendName);
				
		// ��֯sql���
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

	//ͨ���û�����ȡid
	int getid(string name)
	{
		std::cout << "FriendModel::getid" << std::endl;
		char sql[1024] = { 0 };
		sprintf(sql, "select id from user where name='%s'", name.c_str());

		int id = 0;
		MySQL mysql;		
		if (mysql.connect())
		{
			// ע��MYSQL_RESָ����Զ��Ϊ�գ������Ƿ��ѯ������
			MYSQL_RES* res = mysql.query(sql);
			if (res != nullptr)
			{
				MYSQL_ROW row = mysql_fetch_row(res);  //ͨ�����ݿ��ȡid
				// MYSQL_ROW��char**��ͨ��row�Ƿ�Ϊnullptr���жϲ�ѯ�Ƿ񷵻ؽ��
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
