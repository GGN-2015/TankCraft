#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include <string>

#include "ForDebug-XnException.h"
#include "typedef.h"

namespace Xn {

// ȡ����Դ·��
inline std::wstring GetRootPath() {
  std::wstring root_path;

  LPWSTR path = new wchar[256];
  ulong size = GetModuleFileName(nullptr, path, 256);
  if (size == 0 || size == 256) {
    // ����ʧ�ܻ�·�����ض�
    throw XnException(__LINE__, __FILEW__);
  }
  wchar *last_slash_index = wcsrchr(path, L'\\');
  if (last_slash_index) *(last_slash_index + 1) = L'\0';

  root_path = std::wstring(path);
  delete[] path;
  return root_path;
}

}  // namespace Xn
