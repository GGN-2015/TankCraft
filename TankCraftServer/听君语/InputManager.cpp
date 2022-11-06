#include "InputManager.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "Ìý¾ýÓï.h"

using namespace Xn;

InputManager::InputManager() {}

void InputManager::OnKeyDown(const byte &key) {
  keyboard_manager_.OnKeyPressed(key);
}

void InputManager::OnKeyUp(const byte &key) {
  keyboard_manager_.OnKeyReleased(key);
}

void InputManager::OnChar(const wchar &character) {
  keyboard_manager_.OnChar(character);
}

void InputManager::OnMouseMove(int x, int y) noexcept {
  mouse_manager_.OnMouseMove(x, y);
}

void InputManager::OnMouseLeftPressed(int x, int y) noexcept {
  mouse_manager_.OnLeftPressed(x, y);
}

void InputManager::OnMouseLeftReleased(int x, int y) noexcept {
  mouse_manager_.OnLeftReleased(x, y);
}

void InputManager::OnMouseRightPressed(int x, int y) noexcept {
  mouse_manager_.OnRightPressed(x, y);
}

void InputManager::OnMouseRightReleased(int x, int y) noexcept {
  mouse_manager_.OnRightReleased(x, y);
}

void InputManager::OnMouseWheel(int x, int y, int delta) noexcept {
  mouse_manager_.OnWheelDelta(x, y, delta);
}

void InputManager::OnMouseEnter() noexcept { mouse_manager_.OnMouseEnter(); }

void InputManager::OnMouseLeave() noexcept { mouse_manager_.OnMouseLeave(); }

bool InputManager::IsMouseInWindow() noexcept {
  return mouse_manager_.IsInWindow();
}

void InputManager::ClearState() { keyboard_manager_.ClearState(); }
