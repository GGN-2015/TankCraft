#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <dsound.h>

#include <map>

#include "Component.h"
#include "Math.h"
#include "stdafx.h"
#include "typedef.h"

#pragma comment(lib, "dsound")

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

  Microsoft::WRL::ComPtr<IDirectSoundBuffer> audio_bo_;

  std::map<Int, BulletComponent *> unsynced_bullets_;
  std::map<Int, BulletComponent *> synced_bullets_;
};

}  // namespace TankCraft

}  // namespace Xn
