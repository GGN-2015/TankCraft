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

class Transform_RenderComponent;
class Square_RenderComponent;

namespace TankCraft {

struct UserData;

class GameManagerComponent;
class WallManagerComponent;
class TankManagerComponent;
class BulletManagerComponent;

class UserManagerComponent;

// 以x（Vector2::X）为前方向
class MapManagerComponent : public Component {
  friend class GameManagerComponent;

 public:
  MapManagerComponent(UserManagerComponent *user_manager)
      : Component(L"TanksFight_MapManagerComponent"),
        user_manager_(user_manager) {}

  virtual void OnStart() override;
  virtual void OnUpdate() override;
  virtual void OnDestory() override;

  void SetPos(const Vector2 &pos, const Float &scale);
  void SetTargetPos(const Vector2 &pos, const Float &scale);

 private:
  void SetMap(const wchar *const &map_data, const uint &x_side_length,
              const uint &y_side_length);
  void StartSyncTankState();
  void SetTankState(const Int &usesr_id, const Vector2 &pos,
                    const Float &rotation, const Int &state);
  void EndSyncTankState();
  void TryBindUser(const uint &user_id);
  void StartSyncBulletState();
  void SetBulletState(const uint &id, const Vector2 &pos);
  void EndSyncBulletState();

 private:
  uint x_side_length_ = 1;
  uint y_side_length_ = 1;

  WallManagerComponent *wall_manager = nullptr;
  TankManagerComponent *tank_manager = nullptr;
  BulletManagerComponent *bullet_manager = nullptr;

  UserManagerComponent *user_manager_ = nullptr;

 private:
  Vector2 target_pos_;
  Vector2 target_scale_;

  Square_RenderComponent *render_component_ = nullptr;
};

}  // namespace TankCraft

}  // namespace Xn
