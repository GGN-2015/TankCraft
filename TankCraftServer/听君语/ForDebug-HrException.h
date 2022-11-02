#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include <string>

#include "ForDebug-XnException.h"
#include "stdafx.h"

namespace Xn {

// ��HRESULT����תΪstring����
inline std::string HrToString(HRESULT hr) {
  char str[64] = {};
  sprintf_s(str, "HRESULT of 0x%08X . ", static_cast<uint>(hr));
  return std::string(str);
}

// HRESULT�쳣��
class HrException : public XnException {
 public:
  HrException(int line, const wchar *file, HRESULT hr) noexcept;
  const wchar *What() const noexcept override;
  virtual const wchar *GetType() const noexcept;
  static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
  HRESULT GetErrorCode() const noexcept;
  std::wstring GetErrorString() const noexcept;

 private:
  const HRESULT hr_;
};

#define XN_EXCEPT(hr) HrException(__LINE__, __FILEW__, hr)
#define XN_LAST_EXCEPT(hr) HrException(__LINE__, __FILE__, GetLastError())

// ���HRESULTֵ��ʧ��ֵ�����׳�HrException�쳣���쳣
#define XN_THROW_IF_FAILED(hr) \
  if (FAILED(hr)) throw XN_EXCEPT(hr)

}  // namespace Xn
