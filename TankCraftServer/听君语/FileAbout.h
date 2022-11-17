#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <fstream>
#include <iostream>
#include <string>

#include "ForDebug-XnException.h"
#include "stdafx.h"
#include "typedef.h"

namespace Xn {

// 取得资源路径
inline std::wstring GetRootPath() {
  std::wstring root_path;

  LPWSTR path = new wchar[256];
  ulong size = GetModuleFileName(nullptr, path, 256);
  if (size == 0 || size == 256) {
    // 函数失败或路径被截断
    throw XnException(__LINE__, __FILEW__);
  }
  wchar *last_slash_index = wcsrchr(path, L'\\');
  if (last_slash_index) *(last_slash_index + 1) = L'\0';

  root_path = std::wstring(path);
  delete[] path;
  return root_path;
}

inline void ReadWavFileIntoMemory(const std::wstring &wav_file_name,
                                  byte **const &wav_bytes,
                                  ulong *const &wav_size) {
  std::ifstream f(wav_file_name, std::ios::binary);

  f.seekg(0, std::ios::end);
  int lim = f.tellg();
  *wav_size = lim;

  *wav_bytes = new byte[lim];
  f.seekg(0, std::ios::beg);

  f.read((char *)*wav_bytes, lim);

  f.close();
}

// TODO 这个是错的，我暂时不会改变音量大小
//inline void ReadWavFileIntoMemory(const std::wstring &wav_file_name,
//                                  const float &volume, byte **const &wav_bytes,
//                                  ulong *const &wav_size) {
//  ReadWavFileIntoMemory(wav_file_name, wav_bytes, wav_size);
//
//  byte *const _wav_bytes = *wav_bytes;
//
//  byte *data_offset = (_wav_bytes + 40);
//
//  __int16 *p = (__int16 *)(data_offset + 8);
//  constexpr int t = sizeof(*p);
//  for (int i = 80 / sizeof(*p); i < *wav_size / sizeof(*p); i++) {
//    p[i] = (byte)((float)p[i] * volume);
//  }
//}

}  // namespace Xn
