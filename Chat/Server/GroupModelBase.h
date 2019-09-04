#pragma once

#include "GroupDO.h"
#include "MySQL.h"

// Model��ĳ�����
class GroupModelBase
{
public:
	// ���Ӻ�����Ϣ
	virtual bool add(GroupDO &group) = 0;
};

// Group���Model�����
class GroupModel : public GroupModelBase
{
public:
	bool add(GroupDO& group)
	{
		// ��֯sql���
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