#include "InputManager-MouseManager.h"
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include "stdafx.h"

using namespace Xn;

using Type = MouseManager::Event::Type;

Xn::MouseManager::MouseManager()
    : x_(0),
      y_(0),
      is_left_pressed_(false),
      is_right_pressed_(false),
      is_in_window_(false),
      wheel_delta_(0) {}

std::pair<int, int> MouseManager::GetPos() const noexcept { return {x_, y_}; }

int MouseManager::GetPosX() const noexcept { return x_; }

int MouseManager::GetPosY() const noexcept { return y_; }

bool MouseManager::IsInWindow() const noexcept { return is_in_window_; }

bool MouseManager::IsLeftPressed() const noexcept { return is_left_pressed_; }

bool MouseManager::IsRightPressed() const noexcept { return is_right_pressed_; }

MouseManager::Event MouseManager::Read() noexcept {
  if (buffer_.size() > 0) {
    MouseManager::Event e = buffer_.front();
    buffer_.pop();
    return e;
  } else {
    return MouseManager::Event();
  }
}

void MouseManager::Flush() noexcept { buffer_ = std::queue<Event>(); }

void MouseManager::OnMouseMove(int x, int y) noexcept {
  x_ = x;
  y_ = y;

  buffer_.push(MouseManager::Event(Type::Move, *this));
  TrimBuffer();
}

void MouseManager::OnMouseLeave() noexcept {
  is_in_window_ = false;
  buffer_.push(MouseManager::Event(Type::Leave, *this));
  TrimBuffer();
}

void MouseManager::OnMouseEnter() noexcept {
  is_in_window_ = true;
  buffer_.push(MouseManager::Event(Type::Leave, *this));
  TrimBuffer();
}

void MouseManager::OnLeftPressed(int x, int y) noexcept {
  is_left_pressed_ = true;

  buffer_.push(MouseManager::Event(Type::LeftPress, *this));
  TrimBuffer();
}

void MouseManager::OnLeftReleased(int x, int y) noexcept {
  is_left_pressed_ = false;

  buffer_.push(MouseManager::Event(Type::LeftRelease, *this));
  TrimBuffer();
}

void MouseManager::OnRightPressed(int x, int y) noexcept {
  is_right_pressed_ = true;

  buffer_.push(MouseManager::Event(Type::RightPress, *this));
  TrimBuffer();
}

void MouseManager::OnRightReleased(int x, int y) noexcept {
  is_right_pressed_ = false;

  buffer_.push(MouseManager::Event(Type::RightRelease, *this));
  TrimBuffer();
}

void MouseManager::OnWheelUp(int x, int y) noexcept {
  buffer_.push(MouseManager::Event(Type::WheelUp, *this));
  TrimBuffer();
}

void MouseManager::OnWheelDown(int x, int y) noexcept {
  buffer_.push(MouseManager::Event(Type::WheelDown, *this));
  TrimBuffer();
}

void MouseManager::OnWheelDelta(int x, int y, int delta) noexcept {
  wheel_delta_ += delta;
  while (wheel_delta_ >= WHEEL_DELTA) {
    wheel_delta_ -= WHEEL_DELTA;
    OnWheelUp(x, y);
  }
  while (wheel_delta_ <= WHEEL_DELTA) {
    wheel_delta_ += WHEEL_DELTA;
    OnWheelDown(x, y);
  }
}

void MouseManager::TrimBuffer() noexcept {
  while (buffer_.size() > buffer_size_) buffer_.pop();
}
