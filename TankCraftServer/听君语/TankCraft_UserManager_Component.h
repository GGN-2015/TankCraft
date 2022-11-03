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

class GameManagerComponent;

// ��x��Vector2::X��Ϊǰ����
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
