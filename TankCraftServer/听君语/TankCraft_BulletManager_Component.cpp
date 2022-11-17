#include "TankCraft_BulletManager_Component.h"

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "TankCraft_BulletComponent.h"
#include "听君语.h"

void Xn::TankCraft::BulletManagerComponent::OnStart() {}

void Xn::TankCraft::BulletManagerComponent::OnDestory() {
  unsynced_bullets.clear();
  synced_bullets.clear();
}

void Xn::TankCraft::BulletManagerComponent::StartSyncBulletState() {
  unsynced_bullets = std::move(synced_bullets);
}

void Xn::TankCraft::BulletManagerComponent::SetBulletState(
    const uint &bullet_id, const Vector2 &pos) {
  // 看看子弹有没有
  auto bullet = unsynced_bullets.find(bullet_id);
  BulletComponent *new_bullet;
  if (bullet == unsynced_bullets.end()) {
    // 没有userid子弹，就造一个新子弹
    new_bullet = (BulletComponent *)听君语::Get()
                   .GetObjectManager()
                   ->CreateXnObject(Vector2::ZERO, GetXnObject())
                   ->AddComponent(std::make_unique<BulletComponent>());
    new_bullet->SetPos(pos);
  } else {
    // 有，就从未同步列表删去
    new_bullet = bullet->second;
    unsynced_bullets.erase(bullet);
    if (!new_bullet->GetXnObject()->IsActive()) {
      // 未初始化子弹，初始化一下
      new_bullet->GetXnObject()->SetActive(true);
      new_bullet->SetPos(pos);
    } else
      // 正常运行的子弹，继续走吧
      new_bullet->SetTargetPos(pos);
  }
  // 插入到已同步列表中
  synced_bullets.insert({bullet_id, new_bullet});
}

void Xn::TankCraft::BulletManagerComponent::EndSyncBulletState() {
  for (auto &item : unsynced_bullets) {
    item.second->GetXnObject()->SetActive(false);
    synced_bullets.insert({item.first, item.second});
  }
  unsynced_bullets.clear();
}
