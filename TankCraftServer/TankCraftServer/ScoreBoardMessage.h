#include "IMessage.h"

#include <vector>

#include "Utils.h"

#define USER_SCORE_LIST_LEN (10)

struct UserScore {
  int userId;
  unsigned short killCnt; /* 考虑到击杀数不会很多 */

  UserScore(int nUserId, unsigned short nKillCnt)
      : userId(nUserId), killCnt(nKillCnt) {}
};

typedef std::vector<UserScore> UserScoreList;

class ScoreBoardMessage : public IMessage {
 public:
  void AddUserScore(UserScore nUserScore); /* 添加一个用户成绩 */
  void SortUserScore();                    /* 为用户成绩排序 */

  ScoreBoardMessage(unsigned short nThisUserKillCnt)
      : mThisUserKillCnt(nThisUserKillCnt),
        mScoreBoardTime(Utils::GetClockTime()){}

  double GetScoreBoardTime() const;
  void CutUserScore(); /* 剪切分数板到分数板人数上线 */

  virtual void GetRawData(TcpData* tcpData); /* 获取二进制数据 */
  virtual int GetRawDataLength();
  virtual void Dispatch(ThreadBuffer*, GameDatabase*);

  virtual void DebugShow();

 private:
  UserScoreList mUserScoreList;
  unsigned short mThisUserKillCnt;

  double mScoreBoardTime;
};
