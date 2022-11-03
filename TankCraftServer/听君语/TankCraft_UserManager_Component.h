#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Component.h"
#include "Math.h"
#include "typedef.h"

namespace Xn {

namespace TankCraft {

class GameManagerComponent;

// 以x（Vector2::X）为前方向
class UserManagerComponent : public Component {
  friend class GameManagerComponent;

 public:
  UserManagerComponent() : Component(L"TanksFight_UserManagerComponent") {}

  virtual void OnStart() override;
  virtual void OnUpdate() override;
  virtual void OnDestory() override;
};

}  // namespace TankCraft

}  // namespace Xn
