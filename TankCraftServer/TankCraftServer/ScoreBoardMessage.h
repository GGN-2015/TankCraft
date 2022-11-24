#include "IMessage.h"

#include <vector>

#include "Utils.h"

#define USER_SCORE_LIST_LEN (10)

struct UserScore {
  int userId;
  unsigned short killCnt; /* ���ǵ���ɱ������ܶ� */

  UserScore(int nUserId, unsigned short nKillCnt)
      : userId(nUserId), killCnt(nKillCnt) {}
};

typedef std::vector<UserScore> UserScoreList;

class ScoreBoardMessage : public IMessage {
 public:
  void AddUserScore(UserScore nUserScore); /* ���һ���û��ɼ� */
  void SortUserScore();                    /* Ϊ�û��ɼ����� */

  ScoreBoardMessage(unsigned short nThisUserKillCnt)
      : mThisUserKillCnt(nThisUserKillCnt),
        mScoreBoardTime(Utils::GetClockTime()){}

  double GetScoreBoardTime() const;
  void CutUserScore(); /* ���з����嵽�������������� */

  virtual void GetRawData(TcpData* tcpData); /* ��ȡ���������� */
  virtual int GetRawDataLength();
  virtual void Dispatch(ThreadBuffer*, GameDatabase*);

  virtual void DebugShow();

 private:
  UserScoreList mUserScoreList;
  unsigned short mThisUserKillCnt;

  double mScoreBoardTime;
};
