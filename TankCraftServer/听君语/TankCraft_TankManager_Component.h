#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include <map>

#include "Component.h"
#include "Math.h"
#include "typedef.h"

namespace Xn {

namespace TankCraft {

class MapManagerComponent;
class TankComponent;

// ��x��Vector2::X��Ϊǰ����
class TankManagerComponent : public Component {
  friend class MapManagerComponent;

 public:
  TankManagerComponent() : Component(L"TanksFight_TankManagerComponent") {}

  virtual void OnStart() override;

 private:
  void StartSyncTankState();
  void SetTankState(const Int &usesr_id, const Vector2 &pos,
                    const Float &rotation, const Int &state);
  void EndSyncTankState();

  std::map<Int, TankComponent *> unsynced_tanks;
  std::map<Int, TankComponent *> synced_tanks;
};

}  // namespace TankCraft

}  // namespace Xn
