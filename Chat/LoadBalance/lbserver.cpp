#include "lbserver.h"
#include "CMD5Util.h"
#include <vector>

// ���ؾ�������ά��server���б� 127.0.0.1:9000
extern std::vector<std::string> _serverList;

// �󶨵Ļص����������������ӻ������ж�ʱ����
void LBServer::onConnection(const muduo::net::TcpConnectionPtr &con)
{
	if (con->connected()) // �пͻ�������
	{
		// ���ݿͻ��˵�ip:port�ż���һ����ɢ��md5ֵ
		long md5val = getHashVal(con->peerAddress().toIpPort().c_str());
		// ͨ����ͨ��ϣ�㷨����һ��server��ip:port��������
		int index = md5val % _serverList.size();
		con->send(_serverList[index].c_str());
		// lbserver�����Ͽ�����
		con->shutdown();
	}
}