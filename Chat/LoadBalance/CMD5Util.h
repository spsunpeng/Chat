#ifndef _CMD5HashFun
#define _CMD5HashFun

#include"CMd5.h"
static long getHashVal(const char * instr)
{
	int i;
	long hash = 0;
	unsigned char digest[16];

	/*����MD5��غ���������instr��MD5�룬����digest*/
	md5_state_t md5state;
	md5_init(&md5state);
	md5_append(&md5state, (const unsigned char *)instr, strlen(instr));
	md5_finish(&md5state, digest);

	/* ÿ�ĸ��ֽڹ���һ��32λ������
	���ĸ�32λ������ӵõ�instr��hashֵ����������� */
	for (i = 0; i < 4; i++)
	{
		hash += ((long)(digest[i * 4 + 3] & 0xFF) << 24)
			| ((long)(digest[i * 4 + 2] & 0xFF) << 16)
			| ((long)(digest[i * 4 + 1] & 0xFF) << 8)
			| ((long)(digest[i * 4 + 0] & 0xFF));
	}
	return hash;
}
#endif