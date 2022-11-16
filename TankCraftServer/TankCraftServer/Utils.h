#pragma once
#include <string>

#include "TcpData.h"

struct Vector2d { /* ��ά���� */
  double posX, posY;
};

class Utils {
 public:
  static long long GetRandLongLong(); /* ������������� */
  static unsigned short GetUnsignedShort(const char* charArr,
                                         int pos); /* ��ȡ�޷��Ŷ����� */
  static void DumpUnsignedShortToBuffer(
      char* buffer, int pos, unsigned short val); /* װ�ض������������� */
  static void DumpTcpDataToBuffer(
      char* buffer, int pos,
      const TcpData* tcpData); /* �������װ����һ�������� */
  static void GetSanityInteger(const char* buffer, int len, char* evenAns,
                               char* oddAns); /* �������� */
  static double GetClockTime(); /* ��ȡ��������ǰʱ�� (���������˶�����) */
  static std::wstring GetWstringFromCharBuffer(
      const char* buf, int charCnt); /* charCnt ������ż�� */
  static void DumpRawStringToBuffer(char* buf, int pos, const char* raw,
                                    int len); /* �ַ������� */
  static void DumpIntToBuffer(char* buf, int pos, int val); /* �������� */
  static double GetRandomDouble(); /* �������ʵ�� */
  static char GetCharFromBools(bool b0, bool b1, bool b2, bool b3, bool b4,
                               bool b5, bool b6,
                               bool b7);      /* �� bool ��� char */
  static unsigned char GetRandUnsignedChar(); /* �������޷����ַ� */
  static void DumpFloatToBuffer(char* buf, int pos, float fVal);
  static float GetFloatFromBuffer(const char* buf, int pos);
  static double Get2PI();
  static void UnifyDirection(double* dirD); /* ��֤�Ƕ��� 0 ~ 2PI �� */
  static void PointPush(double* px, double* py, double x, double y, double r);
  static double PointDistance(double x1, double y1, double x2, double y2);
  static double VectorLength(double dx, double dy);
  static double GetDirByDxDy(double dx, double dy); /* ����õ��������� */

 private:
  Utils();
};
