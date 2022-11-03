#pragma once
#include <string>

struct UserColor { /* �����û�����ɫ */
	unsigned char R, G, B, A;
	UserColor() { R = G = B = 0; A = 255; } /* Ĭ����ɫΪ��ɫ */
	UserColor(unsigned char nR, unsigned char nG, unsigned char nB, unsigned char nA);

	static UserColor GetRandomColor();
};

class TcpData;

/* ÿһ��������Ϸ��������û�����Ӧ��һ�� UserInfo, ����Ϣ�洢�� GameDatabase �� mUserInfoList ֮�� */
class UserInfo
{
public:
	UserInfo(int nUserId);

	int GetUserId() const;                    /* ��ȡ�û� ID */
	std::wstring GetUserName() const;         /* ��ȡ�û��� */
	void SetUserName(std::wstring nUserName); /* �����û��� */
	void IncKillCnt(int incVal);              /* ������ɱ�� */

	/* ����̹����ɫ */
	void SetUserColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A);
	void SetUserColor(UserColor uc);

	/* ��ȡ�û���Ӧ�� TcpData ����, ������û���Ϣ��ʽ�� */
	void GetUserInfoTcpData(TcpData* tcpData) const;

private:
	int mUserId;
	std::wstring mUserName; /* ��¼�� */

	int mKillCnt;
	UserColor mUserColor;
};
