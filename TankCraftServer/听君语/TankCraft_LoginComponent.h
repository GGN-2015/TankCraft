#pragma once

#include "Component.h"

namespace Xn {

class InputTextComponent;

namespace TankCraft {

class GameManagerComponent;

class LoginComponent : public Component {
 public:
  LoginComponent(GameManagerComponent* const& game_manager)
      : Component(L"TankCraft_LoginComponent"), game_manager_(game_manager) {}

  virtual void OnStart() override;
  virtual void OnUpdate() override;
  virtual void OnDestory() override;

 private:
  GameManagerComponent* game_manager_ = nullptr;
  InputTextComponent* name_input_layout = nullptr;
  InputTextComponent* ipv4_input_layout = nullptr;
  InputTextComponent* port_input_layout = nullptr;
};

}  // namespace TankCraft
}  // namespace Xn
