#include "IMessage.h"

#include <vector>

struct UserScore {
  int userId;
  unsigned short killCnt; /* 考虑到击杀数不会很多 */
};

bool UserScoreCmp(const UserScore&, const UserScore&); /* 用于排序的比较函数 */

typedef std::vector<UserScore> UserScoreList;

class ScoreBoardMessage : public IMessage {
 public:
  void AddUserScore(UserScore nUserScore); /* 添加一个用户成绩 */

  ScoreBoardMessage(unsigned short nThisUserKillCnt)
      : mThisUserKillCnt(nThisUserKillCnt){};

  virtual void GetRawData(TcpData* tcpData); /* 获取二进制数据 */
  virtual int GetRawDataLength();
  virtual void Dispatch(ThreadBuffer*, GameDatabase*);

  virtual void DebugShow();

 private:
  UserScoreList mUserScoreList;
  unsigned short mThisUserKillCnt;
};
