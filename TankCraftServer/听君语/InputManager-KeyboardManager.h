#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <bitset>
#include <queue>

#include "SingletonBaseClass.h"
#include "typedef.h"

namespace Xn {

class InputManager;

class KeyboardManager {
  friend class InputManager;

 public:
  class Event {
   public:
    enum class Type { Press, Release, Invalid };

   public:
    Event() : type_(Type::Invalid), code_(0) {}
    Event(Type type, byte code) : type_(type), code_(code) {}
    bool IsPress() const noexcept { return type_ == Type::Press; }
    bool IsRelease() const noexcept { return type_ == Type::Release; }
    bool IsValid() const noexcept { return type_ == Type::Invalid; }
    byte GetCode() const noexcept { return code_; }

   private:
    Type type_;
    byte code_;
  };

 public:
  KeyboardManager() = default;

  // Key event相关
  bool IsKeyPressed(byte keycode) const noexcept;
  Event ReadKey() noexcept;
  bool IsKeyBufferEmpty() const noexcept;
  void FlushKey() noexcept;
  // char event相关
  char ReadChar() noexcept;
  bool IsCharBufferEmpty() const noexcept;
  void FlushChar() noexcept;
  void Flush() noexcept;

 private:
  void OnKeyPressed(byte keycode) noexcept;
  void OnKeyReleased(byte keycode) noexcept;
  void OnChar(char character) noexcept;
  void ClearState() noexcept;
  template <typename T>
  static void TrimBuffer(std::queue<T> &buffer) noexcept;

 private:
  static constexpr uint key_count_ = 0x100;
  static constexpr uint buffer_size_ = 16;
  std::bitset<key_count_> keystates_;
  std::queue<Event> key_buffer_;
  std::queue<char> char_buffer_;

  CAN_NOT_COPY_THIS_CLASS(KeyboardManager);
};

template <typename T>
inline void KeyboardManager::TrimBuffer(std::queue<T> &buffer) noexcept {
  while (buffer.size() > buffer_size_) {
    buffer.pop();
  }
}

}  // namespace Xn
