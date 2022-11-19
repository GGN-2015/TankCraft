#pragma once
#include <map>
#include <string>

#define SUPER_ARMOR_TIME (1.0) /* 重生后的无敌时间长度 */

struct UserColor { /* 描述用户的颜色 */
  unsigned char R, G, B, A;
  UserColor() {
    R = G = B = 0;
    A = 255;
  } /* 默认颜色为黑色 */
  UserColor(unsigned char nR, unsigned char nG, unsigned char nB,
            unsigned char nA);

  static UserColor GetRandomColor();
};

struct TankPos {           /* 描述坦克的位置和朝向 */
  double posX, posY, dirR; /* X 向右, Y 向下, dir 向右=0, 顺时针弧度制 */

  TankPos() { posX = posY = dirR = 0; /* posX = posY = 0 表示玩家未知未定义 */ }
  TankPos(const TankPos& nTankPos); /* 拷贝构造函数 */

  void RandomPosition(int mHeight, int mWidth); /* 随机坦克位置 */
  void SetPosByAnotherTankPos(const TankPos* pTankPos);

  void SetX(double nPosX); /* 设置坦克的位置和方向 */
  void SetY(double nPosY);
  void SetD(double nDirD);
};

#define TANK_KEY_DOWN (0)
#define TANK_KEY_UP (1)
#define TANK_SHOOT (0)
#define USER_BULLET_MAX (5)
#define TANK_SHOOT_TIME_PERIOD (0.2) /* 坦克发炮冷却时间 */

struct KeyStatus { /* 描述坦克的键盘操作状态 */
  bool shoot;
  bool turnRight;
  bool forward;
  bool turnLeft;
  bool backward;

  KeyStatus();

  bool& GetStatusById(int mStatusId);
  bool GetStatusById(int mStatusId) const;
  bool TurnLeft() const; /* 获取坦克的移动方向 */
  bool TurnRight() const;
  bool MoveForward() const;
  bool MoveBackward() const;
};

class TcpData;
typedef std::map<int, TankPos> TankPosMap; /* 从用户 ID 到坦克位置的映射 */
typedef std::map<int, KeyStatus> KeyStatusMap; /* 从用户 ID 到键盘状态的映射 */

/* 每一个正在游戏中游玩的用户都对应着一个 UserInfo, 该信息存储与 GameDatabase 的
 * mUserInfoList 之中 */
class UserInfo {
 public:
  UserInfo(int nUserId);

  int GetUserId() const;                        /* 获取用户 ID */
  std::wstring GetUserInfoName() const;         /* 获取用户名 */
  void SetUserInfoName(std::wstring nUserName); /* 设置用户名 */
  void IncKillCnt(int incVal);                  /* 新增击杀数 */

  /* 设置坦克颜色 */
  void SetUserColor(unsigned char R, unsigned char G, unsigned char B,
                    unsigned char A);
  void SetUserColor(UserColor uc);
  void Killed(int nHeight, int nWidth); /* 被杀死了，然后随机生成新的位置 */

  /* 获取用户对应的 TcpData 数据, 详见“用户信息格式” */
  void GetUserInfoTcpData(TcpData* tcpData) const;

  void GetTankPos(TankPosMap* nTankPosMap); /* 获取所有玩家的坦克位置 */
  void GetTankPos(double& posX, double& posY) const; /* 获取所有玩家的坦克位置 */
  void GetTankKeyStatus(KeyStatusMap* nKeyStatusMap); /* 获取玩家键盘操作状态 */
  void SetTankPos(const TankPosMap* nTankPosMap);   /* 更新坦克位置 */
  void SetTankPosRandomly(int mHeight, int mWidth); /* 随机更新坦克位置 */

  int GetTankStatus() const; /* 获取坦克状态：无敌/持有激光武器 */
  bool CheckSuperArmor() const; /* 检测玩家是否无敌 */

  void GetTankPosTcpData(TcpData* pTcpData) const; /* ID4 位置8 方向4 状态2 */
  KeyStatus* GetKeyStatusObject();                 /* 获取键盘状态 */

  bool CanShoot() const;
  void Shoot();
  void BulletExpired(int bulletCnt); /* 子弹过期了 */
  void AddBullet(int bulletCnt); /* 子弹因为打死了别人而消失了 */

 private:
  int mUserId;
  std::wstring mUserName; /* 登录名 */

  int mKillCnt;         /* 击杀数 */
  UserColor mUserColor; /* 用户颜色 */
  TankPos mTankPos;     /* 坦克位置 */
  double mLastKilledTime; /* 上次被杀死的时间，根据这个数计算是否处于无敌状态 */
  KeyStatus mKeyStatus; /* 键盘状态 */

  bool mHasLaserWeapon = 0;
  int mBulletCount = 0;
  double mLastShootTime = 0;
};
