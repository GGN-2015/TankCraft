#pragma once

#include "Component.h"

namespace Xn {

class Text_RenderComponent;

namespace TankCraft {

class LoginComponent : public Component {
 public:
  LoginComponent() : Component(L"TankCraft_LoginComponent") {}

  virtual void OnStart() override;
  virtual void OnUpdate() override;
  virtual void OnDestory() override;

 private:
  std::wstring text_;
  Text_RenderComponent* render_component_ = nullptr;
};

}  // namespace TankCraft
}  // namespace Xn
