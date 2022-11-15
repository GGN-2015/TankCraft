#include "TcpData.h"

#include <string.h>

#include <cassert>
#include <iostream>

#include "Utils.h"
#include "TcpDataManager.h"

TcpData* TcpData::AllocTcpData(std::string nFileName, int nLine, bool Test) {
  TcpData* pTcpData = new TcpData;

#ifdef DEBUG_TCP_DATA_ON
  if (Test) {
    TcpDataManager* tdm = TcpDataManager::GetGlobalTcpDataManager();
    tdm->Register(pTcpData, nFileName, nLine);
  }
#endif

  return pTcpData;
}

void TcpData::SetData(const char* data, int len) {
  FreeData(); /* 释放原先的数据 */

  /* len 小于 0 时申请内存会报错 */
  if (len >= 0) {
    mLength = len;
    try {
      mRawData = new char[len];
    } catch (...) {
      std::cerr << "TcpData::SetData() MLE, len = " << len << std::endl;
      assert(false);
    }
    memcpy(mRawData, data, len);
  }
}

TcpData::TcpData() {
  /* 注册 */
  // TcpDataManager* tdm = TcpDataManager::GetGlobalTcpDataManager();
  // tdm->Register(this);

  mRawData = nullptr;
  mLength = 0;
}

TcpData::~TcpData() {  
  FreeData();

#ifdef DEBUG_TCP_DATA_ON
  TcpDataManager* tdm = TcpDataManager::GetGlobalTcpDataManager();
  tdm->Delete(this);
#endif
}

const char* TcpData::GetData() const { return mRawData; }

int TcpData::GetLength() const { return mLength >= 0 ? mLength : 0; }

bool TcpData::IsEnd() const { return GetLength() == 0; }

bool TcpData::GetSanity() const {
  char oddAns = 0, evenAns = 0;
  for (int i = 0; i < GetLength(); i += 1) {
    /* 下标为偶数 */
    if ((i & 1) == 0) {
      evenAns ^= GetData()[i];
    } else {
      oddAns ^= GetData()[i];
    }
  }
  return evenAns == 0 && oddAns == 0; /* 偶校验 */
}

void TcpData::FreeData() {
  if (mRawData != nullptr) { /* 安全释放 */
    delete[] mRawData;
  }
  mRawData = nullptr;
  mLength = 0;
}

void TcpData::DebugShow(const char* prompt) const {
  std::cerr << prompt;
  for (int i = 0; i < GetLength(); i++) {
    std::cerr << (unsigned int)(*(unsigned char*)(&GetData()[i])) << " ";
  }
  std::cerr << std::endl;
}

unsigned short TcpData::GetUnsignedShortAt(int pos) const {
  assert(pos + 1 < GetLength());
  return Utils::GetUnsignedShort(GetData(), pos);
}

void TcpData::IgnoreData() {
  mLength = 0;
  mRawData = nullptr; /* 丢弃原来的数据 */
}

void TcpData::DirectSet(char* nData, int nLen) {
  FreeData();

  mRawData = nData;
  mLength = nLen;
}
