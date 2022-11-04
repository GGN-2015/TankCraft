#pragma once
#include <map>
#include <string>

struct UserColor { /* �����û�����ɫ */
	unsigned char R, G, B, A;
	UserColor() { R = G = B = 0; A = 255; } /* Ĭ����ɫΪ��ɫ */
	UserColor(unsigned char nR, unsigned char nG, unsigned char nB, unsigned char nA);

	static UserColor GetRandomColor();
};

struct TankPos {
	float posX, posY, dirR; /* X ����, Y ����, dir ����=0, ˳ʱ�뻡���� */

	TankPos() {
		posX = posY = dirR = 0; /* posX = posY = 0 ��ʾ���δ֪δ���� */
	}
	TankPos(const TankPos& nTankPos); /* �������캯�� */

	void RandomPosition(int mHeight, int mWidth); /* ���̹��λ�� */
};

class TcpData;
typedef std::map<int, TankPos> TankPosMap; /* ���û� ID ��̹��λ�õ�ӳ�� */

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

	void GetTankPos(TankPosMap* nTankPosMap);         /* ��ȡ������ҵ�̹��λ�� */
	void SetTankPos(const TankPosMap* nTankPosMap);   /* ����̹��λ�� */
	void SetTankPosRandomly(int mHeight, int mWidth); /* �������̹��λ�� */

private:
	int mUserId;
	std::wstring mUserName; /* ��¼�� */

	int       mKillCnt  ; /* ��ɱ�� */
	UserColor mUserColor; /* �û���ɫ */
	TankPos   mTankPos  ; /* ̹��λ�� */
	double    mLastKilledTime; /* �ϴα�ɱ����ʱ�� */
};
