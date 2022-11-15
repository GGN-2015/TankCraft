#pragma once

#include <string>

// #define DEBUG_TCP_DATA_ON

class TcpData {
 public:
  ~TcpData();
  static TcpData* AllocTcpData(std::string nFileName, int nLine, bool = true);

  void SetData(const char* data, int len); /* ���� ���� */
  const char* GetData() const;             /* ��ȡԭʼ���� */
  int GetLength() const;                   /* ��ȡ���ݳ��� */
  bool IsEnd() const;                      /* ����Ƿ��ǽ�β�� */
  bool GetSanity() const;                  /* ���У��� */
  void FreeData();

  void DebugShow(const char* prompt) const; /* ���ڶ����������ݵ���ʾ */

  unsigned short GetUnsignedShortAt(int pos) const; /* ��ȡһ���޷��Ŷ����� */
  void IgnoreData(); /* ֱ�Ӷ�����ǰ�����ݵ��ǲ����� */

  void DirectSet(char* nData, int nLen); /* ֱ�ӹ���һ������ */

 private:
  TcpData();

  int mLength;
  char* mRawData;
};
