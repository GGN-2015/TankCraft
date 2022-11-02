#include "ForDebug-HrException.h"
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

using namespace Xn;

HrException::HrException(int line, const wchar *file, HRESULT hr) noexcept
    : XnException(line, file), hr_(hr) {}

const wchar *HrException::What() const noexcept {
  std::wostringstream out_string_stream;
  out_string_stream << GetType() << '\n'
                    << "[Error Code]" << GetErrorCode() << '\n'
                    << "[Description]" << GetErrorString() << '\n'
                    << GetOriginString();
  what_buffer_ = out_string_stream.str();
  return what_buffer_.c_str();
}

const wchar *HrException::GetType() const noexcept { return L"XnHrException"; }

std::wstring HrException::TranslateErrorCode(HRESULT hr) noexcept {
  wchar *message_buffer = nullptr;
  DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS;
  DWORD message_length = FormatMessage(
      flags, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      reinterpret_cast<LPWSTR>(message_buffer), 0, nullptr);
  if (message_length == 0) return L"Unidentified error code";
  std::wstring error_string = message_buffer;
  LocalFree(message_buffer);
  return error_string;
}

HRESULT HrException::GetErrorCode() const noexcept { return hr_; }

std::wstring HrException::GetErrorString() const noexcept {
  return TranslateErrorCode(hr_);
}
