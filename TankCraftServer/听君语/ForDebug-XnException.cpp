#include "ForDebug-XnException.h"
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include <sstream>

using namespace Xn;

XnException::XnException(uint line, const wchar *file) noexcept
    : line_(line), file_(file) {}

const wchar *XnException::What() const noexcept {
  std::wostringstream out_string_stream;
  out_string_stream << GetType() << '\n' << GetOriginString().c_str();
  what_buffer_ = out_string_stream.str();
  return what_buffer_.c_str();
}

const wchar *XnException::GetType() const noexcept { return L"XnException"; }
int XnException::GetLine() const noexcept { return line_; }
const std::wstring &XnException::GetFile() const noexcept { return file_; }

std::wstring XnException::GetOriginString() const noexcept {
  std::wostringstream out_string_stream;
  out_string_stream << "[File]" << file_ << '\n' << "[Line]" << line_;
  return out_string_stream.str();
}
