#pragma once
#include <string>

#include "TcpData.h"

struct Vector2d { /* 二维向量 */
  double posX, posY;
};

class Utils {
 public:
  static long long GetRandLongLong(); /* 生成随机的整数 */
  static unsigned short GetUnsignedShort(const char* charArr,
                                         int pos); /* 获取无符号短整数 */
  static void DumpUnsignedShortToBuffer(
      char* buffer, int pos, unsigned short val); /* 装载短整数到二进制 */
  static void DumpTcpDataToBuffer(
      char* buffer, int pos,
      const TcpData* tcpData); /* 向二进制装载另一个二进制 */
  static void GetSanityInteger(const char* buffer, int len, char* evenAns,
                               char* oddAns); /* 计算异或和 */
  static double GetClockTime(); /* 获取服务器当前时刻 (开机运行了多少秒) */
  static std::wstring GetWstringFromCharBuffer(
      const char* buf, int charCnt); /* charCnt 必须是偶数 */
  static void DumpRawStringToBuffer(char* buf, int pos, const char* raw,
                                    int len); /* 字符串载入 */
  static void DumpIntToBuffer(char* buf, int pos, int val); /* 整数载入 */
  static double GetRandomDouble(); /* 生成随机实数 */
  static char GetCharFromBools(bool b0, bool b1, bool b2, bool b3, bool b4,
                               bool b5, bool b6,
                               bool b7);      /* 从 bool 获得 char */
  static unsigned char GetRandUnsignedChar(); /* 获得随机无符号字符 */
  static void DumpFloatToBuffer(char* buf, int pos, float fVal);
  static float GetFloatFromBuffer(const char* buf, int pos);
  static double Get2PI();
  static void UnifyDirection(double* dirD); /* 保证角度在 0 ~ 2PI 内 */
  static void PointPush(double* px, double* py, double x, double y, double r);
  static double PointDistance(double x1, double y1, double x2, double y2);
  static double VectorLength(double dx, double dy);
  static double GetDirByDxDy(double dx, double dy); /* 计算得到弧度坐标 */

 private:
  Utils();
};
