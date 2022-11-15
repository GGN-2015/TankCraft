#include "TcpDataManager.h"

#include <cassert>
#include <iostream>

#include "Utils.h"

TcpDataManager* TcpDataManager::sGlobalTcpDataManager = nullptr;

TcpDataManager* TcpDataManager::GetGlobalTcpDataManager() {
  if (sGlobalTcpDataManager == nullptr) {
    sGlobalTcpDataManager = new TcpDataManager;
  }
  return sGlobalTcpDataManager;
}

bool TcpDataManager::Register(TcpData* pTcpData, std::string nFileName, int nLine) {
  // std::cerr << "TcpDataManager::Register cntNow = " << mTcpDataMap.size()
  //          << std::endl;

#ifdef DEBUG_TCP_DATA_ON
  DeleteDeadTcpData();
#endif

  if (mTcpDataMap.count((void*)pTcpData) == 0) {
    mTcpDataMap[(void*)pTcpData] = {Utils::GetClockTime(), nFileName, nLine};
    return true;
  } else {
    return false; /* 重复插入 */
  }
}

void TcpDataManager::Delete(TcpData* pTcpData) {
  if (mTcpDataMap.count((void*)pTcpData) != 0) {
    mTcpDataMap.erase((void*)pTcpData);
  }
}

TcpDataManager::TcpDataManager() {

}

void TcpDataManager::DeleteDeadTcpData() {
  TcpDataList tcpDataList;
  double timeNow = Utils::GetClockTime();
  for (auto& pTcpData : mTcpDataMap) {
    if ((timeNow - pTcpData.second.runTime) >= TCPDATA_MAX_SURVIVE_TIME) {
      tcpDataList.push_back(pTcpData.first);
    }
  }

#ifdef TCP_DATA_ASSERTION
  for (auto pTcpData : tcpDataList) {
    std::cerr << "__FILE__ = " << mTcpDataMap[pTcpData].fileName
              << " __LINE__ = " << mTcpDataMap[pTcpData].line << std::endl;
  }
  if (tcpDataList.size() != 0) {
    system("pause");
    assert(false); /* 不允许程序继续执行 */
  }
#endif
}
