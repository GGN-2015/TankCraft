
#include "ScoreBoardMessage.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "TcpData.h"
#include "Utils.h"

void ScoreBoardMessage::AddUserScore(UserScore nUserScore) {
  mUserScoreList.push_back(nUserScore);
}

/* ��ɱ����һ�ؼ����û� id �ڶ��ؼ������� */
static bool UserScoreCmp(const UserScore &us1, const UserScore &us2) {
  if (us1.killCnt != us2.killCnt) {
    return us1.killCnt > us2.killCnt;
  }
  return us1.userId < us2.userId;
}

void ScoreBoardMessage::SortUserScore() { 
    std::sort(mUserScoreList.begin(), mUserScoreList.end(), UserScoreCmp); 
}

double ScoreBoardMessage::GetScoreBoardTime() const { 
    return mScoreBoardTime; 
}

void ScoreBoardMessage::CutUserScore() {
  while (mUserScoreList.size() > USER_SCORE_LIST_LEN) mUserScoreList.pop_back();
}

void ScoreBoardMessage::GetRawData(TcpData *tcpData) {
  char *buf = new char[GetRawDataLength()];
  int pos = 0;

  Utils::DumpUnsignedShortToBuffer(buf, pos, 5); /* �����Ϣ */
  pos += 2;

  Utils::DumpUnsignedShortToBuffer(buf, pos,
                                   GetRawDataLength() - 4); /* ���ݲ��ֳ��� */
  pos += 2;

  Utils::DumpUnsignedShortToBuffer(buf, pos,
                                   mThisUserKillCnt); /* ��ǰ��һ�ɱ�� */
  pos += 2;

  Utils::DumpUnsignedShortToBuffer(buf, pos,
                                   (unsigned short)mUserScoreList.size()); /* ���а��е������� */
  pos += 2;

  for (int i = 0; i < mUserScoreList.size(); i += 1) {
    int userId             = mUserScoreList[i].userId;
    unsigned short killCnt = mUserScoreList[i].killCnt;

    Utils::DumpIntToBuffer(buf, pos, userId); /* �û� id */
    pos += 4;

    Utils::DumpUnsignedShortToBuffer(buf, pos, killCnt); /* ��ɱ�� */
    pos += 2;
  }

  tcpData->SetData(buf, GetRawDataLength());
  delete[] buf;
}

int ScoreBoardMessage::GetRawDataLength() {
  return 2 + 2 + 2 + 2 + 6 * (int)mUserScoreList.size();
}

void ScoreBoardMessage::Dispatch(ThreadBuffer *, GameDatabase *) {
  assert(false);
}

void ScoreBoardMessage::DebugShow() {
  std::cerr << "[ScoreBoardMessage::DebugShow]" << std::endl;
  for (int i = 0; i < mUserScoreList.size(); i += 1) {
    std::cerr << "\t"
              << "UserId = " << mUserScoreList[i].userId << "\t"
              << "KillCnt = " << mUserScoreList[i].killCnt << std::endl;
  }
}
