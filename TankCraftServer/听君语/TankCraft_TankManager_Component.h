#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "Component.h"
#include "Math.h"
#include "typedef.h"

namespace Xn {

namespace TankCraft {

class MapManagerComponent;

// ��x��Vector2::X��Ϊǰ����
class TankManagerComponent : public Component {
  friend class MapManagerComponent;

 public:
  TankManagerComponent() : Component(L"TanksFight_TankManagerComponent") {}

  virtual void OnStart() override;
};

}  // namespace TankCraft

}  // namespace Xn
