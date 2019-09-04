#pragma once

#pragma once

#include "MySQL.h"
#include<iostream>

// Model��ĳ�����
class groupUserModelBase
{
public:
	// ���Ӻ�����Ϣ
	virtual bool add(string groupName, int userid, string grouprole) = 0;
};

// Friend���Model�����
class groupUserModel : public groupUserModelBase
{
public:
	bool add(string groupName, int userid, string grouprole)
	{
		int groupid = getid(groupName);

		// ��֯sql���
		char sql[1024] = { 0 };
		sprintf(sql, "insert into groupuser(groupid,userid,grouprole) values(%d,%d,'%s')"
			, groupid, userid, grouprole.c_str());

		MySQL mysql;
		if (mysql.connect())
		{
			if (mysql.update(sql))
			{
				LOG_INFO << "add groupUser success => sql:" << sql;
				return true;
			}
		}
		LOG_INFO << "add groupUser error => sql:" << sql;
		return false;
	}

	//ͨ���û�����ȡid
	int getid(string name)
	{
		char sql[1024] = { 0 };
		sprintf(sql, "select id from allgroup where groupname='%s'", name.c_str());

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
					mysql_free_result(res);
				}
			}
		}
		return id;
	}
};

