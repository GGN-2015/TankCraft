#pragma once
#include <map>
#include <string>

#define SUPER_ARMOR_TIME (1.0) /* ��������޵�ʱ�䳤�� */

struct UserColor { /* �����û�����ɫ */
  unsigned char R, G, B, A;
  UserColor() {
    R = G = B = 0;
    A = 255;
  } /* Ĭ����ɫΪ��ɫ */
  UserColor(unsigned char nR, unsigned char nG, unsigned char nB,
            unsigned char nA);

  static UserColor GetRandomColor();
};

struct TankPos {           /* ����̹�˵�λ�úͳ��� */
  double posX, posY, dirR; /* X ����, Y ����, dir ����=0, ˳ʱ�뻡���� */

  TankPos() { posX = posY = dirR = 0; /* posX = posY = 0 ��ʾ���δ֪δ���� */ }
  TankPos(const TankPos& nTankPos); /* �������캯�� */

  void RandomPosition(int mHeight, int mWidth); /* ���̹��λ�� */
  void SetPosByAnotherTankPos(const TankPos* pTankPos);

  void SetX(double nPosX); /* ����̹�˵�λ�úͷ��� */
  void SetY(double nPosY);
  void SetD(double nDirD);
};

#define TANK_KEY_DOWN (0)
#define TANK_KEY_UP (1)
#define TANK_SHOOT (0)
#define USER_BULLET_MAX (5)
#define TANK_SHOOT_TIME_PERIOD (0.2) /* ̹�˷�����ȴʱ�� */

struct KeyStatus { /* ����̹�˵ļ��̲���״̬ */
  bool shoot;
  bool turnRight;
  bool forward;
  bool turnLeft;
  bool backward;

  KeyStatus();

  bool& GetStatusById(int mStatusId);
  bool GetStatusById(int mStatusId) const;
  bool TurnLeft() const; /* ��ȡ̹�˵��ƶ����� */
  bool TurnRight() const;
  bool MoveForward() const;
  bool MoveBackward() const;
};

class TcpData;
typedef std::map<int, TankPos> TankPosMap; /* ���û� ID ��̹��λ�õ�ӳ�� */
typedef std::map<int, KeyStatus> KeyStatusMap; /* ���û� ID ������״̬��ӳ�� */

/* ÿһ��������Ϸ��������û�����Ӧ��һ�� UserInfo, ����Ϣ�洢�� GameDatabase ��
 * mUserInfoList ֮�� */
class UserInfo {
 public:
  UserInfo(int nUserId);

  int GetUserId() const;                        /* ��ȡ�û� ID */
  std::wstring GetUserInfoName() const;         /* ��ȡ�û��� */
  void SetUserInfoName(std::wstring nUserName); /* �����û��� */
  void IncKillCnt(int incVal);                  /* ������ɱ�� */

  /* ����̹����ɫ */
  void SetUserColor(unsigned char R, unsigned char G, unsigned char B,
                    unsigned char A);
  void SetUserColor(UserColor uc);
  void Killed(int nHeight, int nWidth); /* ��ɱ���ˣ�Ȼ����������µ�λ�� */

  /* ��ȡ�û���Ӧ�� TcpData ����, ������û���Ϣ��ʽ�� */
  void GetUserInfoTcpData(TcpData* tcpData) const;

  void GetTankPos(TankPosMap* nTankPosMap); /* ��ȡ������ҵ�̹��λ�� */
  void GetTankPos(double& posX, double& posY) const; /* ��ȡ������ҵ�̹��λ�� */
  void GetTankKeyStatus(KeyStatusMap* nKeyStatusMap); /* ��ȡ��Ҽ��̲���״̬ */
  void SetTankPos(const TankPosMap* nTankPosMap);   /* ����̹��λ�� */
  void SetTankPosRandomly(int mHeight, int mWidth); /* �������̹��λ�� */

  int GetTankStatus() const; /* ��ȡ̹��״̬���޵�/���м������� */
  bool CheckSuperArmor() const; /* �������Ƿ��޵� */

  void GetTankPosTcpData(TcpData* pTcpData) const; /* ID4 λ��8 ����4 ״̬2 */
  KeyStatus* GetKeyStatusObject();                 /* ��ȡ����״̬ */

  bool CanShoot() const;
  void Shoot();
  void BulletExpired(int bulletCnt); /* �ӵ������� */
  void AddBullet(int bulletCnt); /* �ӵ���Ϊ�����˱��˶���ʧ�� */

 private:
  int mUserId;
  std::wstring mUserName; /* ��¼�� */

  int mKillCnt;         /* ��ɱ�� */
  UserColor mUserColor; /* �û���ɫ */
  TankPos mTankPos;     /* ̹��λ�� */
  double mLastKilledTime; /* �ϴα�ɱ����ʱ�䣬��������������Ƿ����޵�״̬ */
  KeyStatus mKeyStatus; /* ����״̬ */

  bool mHasLaserWeapon = 0;
  int mBulletCount = 0;
  double mLastShootTime = 0;
};
