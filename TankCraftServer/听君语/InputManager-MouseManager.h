#pragma once
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include <queue>

#include "SingletonBaseClass.h"
#include "typedef.h"

namespace Xn {

class InputManager;

class MouseManager {
  friend class InputManager;

 public:
  class Event {
   public:
    enum class Type {
      LeftPress,
      LeftRelease,
      RightPress,
      RightRelease,
      WheelUp,
      WheelDown,
      Move,
      Enter,
      Leave,
      Invalid
    };

   public:
    Event() noexcept
        : type_(Type::Invalid),
          is_left_pressed_(false),
          is_right_pressed_(false),
          x_(0),
          y_(0) {}
    Event(Type type, const MouseManager& parent) noexcept
        : type_(type),
          is_left_pressed_(parent.is_left_pressed_),
          is_right_pressed_(parent.is_right_pressed_),
          x_(parent.x_),
          y_(parent.y_) {}
    bool IsValid() const noexcept { return type_ != Type::Invalid; }
    Type GetType() const noexcept { return type_; }
    std::pair<int, int> GetPos() const noexcept { return {x_, y_}; }
    int GetPosX() const noexcept { return x_; }
    int GetPosY() const noexcept { return y_; }
    bool IsLeftPressed() const noexcept { return is_left_pressed_; }
    bool IsRightPressed() const noexcept { return is_right_pressed_; }

   private:
    Type type_;
    bool is_left_pressed_;
    bool is_right_pressed_;
    int x_;
    int y_;
  };

 private:
  static constexpr uint buffer_size_ = 16;

 public:
  MouseManager();
  std::pair<int, int> GetPos() const noexcept;
  int GetPosX() const noexcept;
  int GetPosY() const noexcept;
  bool IsInWindow() const noexcept;
  bool IsLeftPressed() const noexcept;
  bool IsRightPressed() const noexcept;
  Event Read() noexcept;
  bool IsEmpty() const noexcept { return buffer_.empty(); }
  void Flush() noexcept;

 private:
  void OnMouseMove(int x, int y) noexcept;
  void OnMouseLeave() noexcept;
  void OnMouseEnter() noexcept;
  void OnLeftPressed(int x, int y) noexcept;
  void OnLeftReleased(int x, int y) noexcept;
  void OnRightPressed(int x, int y) noexcept;
  void OnRightReleased(int x, int y) noexcept;
  void OnWheelUp(int x, int y) noexcept;
  void OnWheelDown(int x, int y) noexcept;
  void OnWheelDelta(int x, int y, int delta) noexcept;
  void TrimBuffer() noexcept;

 private:
  int x_;
  int y_;
  bool is_left_pressed_;
  bool is_right_pressed_;
  bool is_in_window_;
  int wheel_delta_;
  std::queue<Event> buffer_;

  CAN_NOT_COPY_THIS_CLASS(MouseManager);
};

}  // namespace Xn
