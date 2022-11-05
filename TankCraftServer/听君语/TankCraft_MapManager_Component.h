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

// 以x（Vector2::X）为前方向
class MapManagerComponent : public Component {
  friend class GameManagerComponent;

 public:
  MapManagerComponent() : Component(L"TanksFight_MapManagerComponent") {}

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
  void TryBindUser(const UserData *const &user_data);

 private:
  uint x_side_length_ = 1;
  uint y_side_length_ = 1;

  WallManagerComponent *wall_manager = nullptr;
  TankManagerComponent *tank_manager = nullptr;

 private:
  Vector2 target_pos_;
  Vector2 target_scale_;

  Square_RenderComponent *render_component_ = nullptr;
};

}  // namespace TankCraft

}  // namespace Xn
