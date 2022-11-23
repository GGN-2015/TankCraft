#include "IMessage.h"

#include <vector>

struct UserScore {
  int userId;
  unsigned short killCnt; /* ���ǵ���ɱ������ܶ� */
};

bool UserScoreCmp(const UserScore&, const UserScore&); /* ��������ıȽϺ��� */

typedef std::vector<UserScore> UserScoreList;

class ScoreBoardMessage : public IMessage {
 public:
  void AddUserScore(UserScore nUserScore); /* ���һ���û��ɼ� */

  ScoreBoardMessage(unsigned short nThisUserKillCnt)
      : mThisUserKillCnt(nThisUserKillCnt){};

  virtual void GetRawData(TcpData* tcpData); /* ��ȡ���������� */
  virtual int GetRawDataLength();
  virtual void Dispatch(ThreadBuffer*, GameDatabase*);

  virtual void DebugShow();

 private:
  UserScoreList mUserScoreList;
  unsigned short mThisUserKillCnt;
};
