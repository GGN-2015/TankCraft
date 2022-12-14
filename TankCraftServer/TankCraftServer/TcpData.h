#pragma once

#include <string>

// #define DEBUG_TCP_DATA_ON

class TcpData {
 public:
  ~TcpData();
  static TcpData* AllocTcpData(std::string nFileName, int nLine, bool = true);

  void SetData(const char* data, int len); /* 设置 数据 */
  const char* GetData() const;             /* 获取原始数据 */
  int GetLength() const;                   /* 获取数据长度 */
  bool IsEnd() const;                      /* 检查是否是结尾符 */
  bool GetSanity() const;                  /* 检查校验和 */
  void FreeData();

  void DebugShow(const char* prompt) const; /* 用于对于数据内容的显示 */

  unsigned short GetUnsignedShortAt(int pos) const; /* 获取一个无符号短整数 */
  void IgnoreData(); /* 直接丢弃当前的数据但是不析构 */

  void DirectSet(char* nData, int nLen); /* 直接过继一个数据 */

 private:
  TcpData();

  int mLength;
  char* mRawData;
};
