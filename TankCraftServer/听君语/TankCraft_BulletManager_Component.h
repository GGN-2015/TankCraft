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

class MapManagerComponent;
class BulletComponent;

// 以x（Vector2::X）为前方向
class BulletManagerComponent : public Component {
  friend class MapManagerComponent;

 public:
  BulletManagerComponent() : Component(L"TanksFight_BulletManagerComponent") {}

  virtual void OnStart() override;
  virtual void OnDestory() override;

 private:
  void StartSyncBulletState();
  void SetBulletState(const uint &bullet_id, const Vector2 &pos);
  void EndSyncBulletState();

  byte *audio_bo_ = nullptr;

  std::map<Int, BulletComponent *> unsynced_bullets_;
  std::map<Int, BulletComponent *> synced_bullets_;
};

}  // namespace TankCraft

}  // namespace Xn
