#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <string>

#include "ForDebug-XnException.h"
#include "stdafx.h"

namespace Xn {

// 将HRESULT数据转为string数据
inline std::string HrToString(HRESULT hr) {
  char str[64] = {};
  sprintf_s(str, "HRESULT of 0x%08X . ", static_cast<uint>(hr));
  return std::string(str);
}

// HRESULT异常类
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

// 如果HRESULT值是失败值，就抛出HrException异常类异常
#define XN_THROW_IF_FAILED(hr) \
  if (FAILED(hr)) throw XN_EXCEPT(hr)

}  // namespace Xn
