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

class UserManagerComponent;

// 以x（Vector2::X）为前方向
class TankManagerComponent : public Component {
  friend class MapManagerComponent;

 public:
  TankManagerComponent(UserManagerComponent *user_manager)
      : Component(L"TanksFight_TankManagerComponent"),
        user_manager_(user_manager) {}

  virtual void OnStart() override;
  virtual void OnUpdate() override;

 private:
  void StartSyncTankState();
  void SetTankState(const Int &user_id, const Vector2 &pos,
                    const Float &rotation, const Int &state);
  void EndSyncTankState();
  void TryBindUser(const uint &user_id);

  std::map<Int, TankComponent *> unsynced_tanks;
  std::map<Int, TankComponent *> synced_tanks;

  UserManagerComponent *user_manager_;
};

}  // namespace TankCraft

}  // namespace Xn
