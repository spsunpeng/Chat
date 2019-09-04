#pragma once

#include "GroupDO.h"
#include "MySQL.h"

// Model层的抽象类
class GroupModelBase
{
public:
	// 增加好友信息
	virtual bool add(GroupDO &group) = 0;
};

// Group表的Model层操作
class GroupModel : public GroupModelBase
{
public:
	bool add(GroupDO& group)
	{
		// 组织sql语句
		char sql[1024] = { 0 };
		sprintf(sql, "insert into allgroup(groupname) values('%s')", group.getName().c_str());


		MySQL mysql;
		if (mysql.connect())
		{
			if (mysql.update(sql))
			{
				LOG_INFO << "add group success => sql:" << sql;
				return true;
			}
		}
		LOG_INFO << "add group error => sql:" << sql;
		return false;
	}
};