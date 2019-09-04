#pragma once

/*
����ͷ�ļ���ͳһ�������Ϳͻ��˵ĳ�������
*/
const int MSG_LOGIN = 0; // login msg
const int MSG_LOGIN_ACK = 1; // login ack msg
const int MSG_REG = 2; // register msg
const int MSG_REG_ACK = 3; // register ack msg
const int MSG_ADD_FRIEND = 4; // add friend msg
const int MSG_ADD_FRIEND_ACK = 5; // add friend ack msg
const int MSG_ADD_GROUP = 6; // add group msg
const int MSG_ADD_GROUP_ACK = 7; // add group ack msg
const int MSG_ONE_CHAT = 8; // one to one chat msg
const int MSG_ONE_CHAT_ACK = 9; // one to one chat ack msg
const int MSG_LOGINOUT = 10; // logout msg
const int MSG_GROUP_CHAT = 11; // group chat msg
const int MSG_GROUP_CHAT_ACK = 12; // group chat ack msg
const int MSG_ACK = 13; // test

const int MSG_ADD_GROUPUSER = 14;         //������Ա
const int MSG_ADD_GROUPUSER_ACK = 15;     //����_������Ա

const int MSG_ONLINEUSER_FILE = 21;       //��ȡ�����û��б�
const int MSG_ONLINEUSER_FILE_ACK = 22;   //����_��ȡ�����û��б�
const int MSG_GROUP_FILE = 23;            //��ȡȺ���б�
const int MSG_GROUP_FILE_ACK = 24;        //����_��ȡȺ���б�
const int MSG_FREEND_FILE = 25;           //��ȡ�����б�
const int MSG_FREEND_FILE_ACK = 26;       //����_��ȡ�����б�

const int ACK_SUCCESS = 100; // msg process ok
const int ACK_ERROR = 101; // msg process err

