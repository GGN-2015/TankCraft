#include "InputManager-KeyboardManager.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

using namespace Xn;

bool KeyboardManager::IsKeyPressed(byte keycode) const noexcept {
  return keystates_[keycode];
}

KeyboardManager::Event KeyboardManager::ReadKey() noexcept {
  if (key_buffer_.size() > 0) {
    Event e = key_buffer_.front();
    key_buffer_.pop();
    return e;
  } else {
    return Event();
  }
}

bool KeyboardManager::IsKeyBufferEmpty() const noexcept {
  return key_buffer_.empty();
}

void KeyboardManager::FlushKey() noexcept { key_buffer_ = std::queue<Event>(); }

wchar KeyboardManager::ReadChar() noexcept {
  if (char_buffer_.size() > 0) {
    wchar charcode = char_buffer_.front();
    char_buffer_.pop();
    return charcode;
  } else {
    return 0;
  }
}

bool KeyboardManager::IsCharBufferEmpty() const noexcept {
  return char_buffer_.empty();
}

void KeyboardManager::FlushChar() noexcept {
  char_buffer_ = std::queue<wchar>();
}

void KeyboardManager::Flush() noexcept {
  FlushKey();
  FlushChar();
}

void KeyboardManager::OnKeyPressed(byte keycode) noexcept {
  keystates_[keycode] = true;
  key_buffer_.push(Event(Event::Type::Press, keycode));
  TrimBuffer(key_buffer_);
}

void KeyboardManager::OnKeyReleased(byte keycode) noexcept {
  keystates_[keycode] = false;
  key_buffer_.push(Event(Event::Type::Release, keycode));
  TrimBuffer(key_buffer_);
}

void KeyboardManager::OnChar(wchar character) noexcept {
  char_buffer_.push(character);
  TrimBuffer(char_buffer_);
}

void KeyboardManager::ClearState() noexcept { keystates_.reset(); }
