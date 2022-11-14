#pragma once

#include <map>
#include <string>
#include <vector>

#define TCPDATA_MAX_SURVIVE_TIME (5.0) /* TCP 数据最大存活 1s */

struct TcpDataInfo {
  double runTime;
  std::string fileName;
  int line;
};
typedef std::map<void*, TcpDataInfo> TcpDataMap;
typedef std::vector<void*> TcpDataList;

class TcpData;
class TcpDataManager {
 public:
  static TcpDataManager* GetGlobalTcpDataManager();

  bool Register(TcpData*, std::string nFileName, int nLine);
  void Delete(TcpData*);

 private:
  TcpDataManager();
  void DeleteDeadTcpData(); /* 清除已经超过生命周期的 TcpData */

  TcpDataMap mTcpDataMap;
  static TcpDataManager* sGlobalTcpDataManager;
};
