#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <map>

#include "Component.h"
#include "Math.h"
#include "typedef.h"

namespace Xn {

namespace TankCraft {

struct UserData;

class MapManagerComponent;
class TankComponent;

// 以x（Vector2::X）为前方向
class TankManagerComponent : public Component {
  friend class MapManagerComponent;

 public:
  TankManagerComponent() : Component(L"TanksFight_TankManagerComponent") {}

  virtual void OnStart() override;

 private:
  void StartSyncTankState();
  void SetTankState(const Int &user_id, const Vector2 &pos,
                    const Float &rotation, const Int &state);
  void EndSyncTankState();
  void TryBindUser(const UserData *const &user_data);

  std::map<Int, TankComponent *> unsynced_tanks;
  std::map<Int, TankComponent *> synced_tanks;
};

}  // namespace TankCraft

}  // namespace Xn
