#pragma once
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include <exception>
#include <string>

#include "typedef.h"

namespace Xn {

class XnException : public std::exception {
 public:
  XnException(uint line, const wchar *file) noexcept;

  virtual const wchar *What() const noexcept;
  virtual const wchar *GetType() const noexcept;
  int GetLine() const noexcept;
  const std::wstring &GetFile() const noexcept;
  std::wstring GetOriginString() const noexcept;

 private:
  int line_;
  std::wstring file_;

 protected:
  mutable std::wstring what_buffer_;
};

#define THROW_XN_ERROR() throw XnException(__LINE__, __FILEW__)

}  // namespace Xn