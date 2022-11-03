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

class MapManagerComponent;

// 以x（Vector2::X）为前方向
class TankManagerComponent : public Component {
  friend class MapManagerComponent;

 public:
  TankManagerComponent() : Component(L"TanksFight_TankManagerComponent") {}

  virtual void OnStart() override;
};

}  // namespace TankCraft

}  // namespace Xn
