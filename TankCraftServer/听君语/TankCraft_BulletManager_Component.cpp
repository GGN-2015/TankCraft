#include "TankCraft_BulletManager_Component.h"

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "TankCraft_BulletComponent.h"
#include "������.h"

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
  // �����ӵ���û��
  auto bullet = unsynced_bullets.find(bullet_id);
  BulletComponent *new_bullet;
  if (bullet == unsynced_bullets.end()) {
    // û��userid�ӵ�������һ�����ӵ�
    new_bullet = (BulletComponent *)������::Get()
                   .GetObjectManager()
                   ->CreateXnObject(Vector2::ZERO, GetXnObject())
                   ->AddComponent(std::make_unique<BulletComponent>());
    new_bullet->SetPos(pos);
  } else {
    // �У��ʹ�δͬ���б�ɾȥ
    new_bullet = bullet->second;
    unsynced_bullets.erase(bullet);
    if (!new_bullet->GetXnObject()->IsActive()) {
      // δ��ʼ���ӵ�����ʼ��һ��
      new_bullet->GetXnObject()->SetActive(true);
      new_bullet->SetPos(pos);
    } else
      // �������е��ӵ��������߰�
      new_bullet->SetTargetPos(pos);
  }
  // ���뵽��ͬ���б���
  synced_bullets.insert({bullet_id, new_bullet});
}

void Xn::TankCraft::BulletManagerComponent::EndSyncBulletState() {
  for (auto &item : unsynced_bullets) {
    item.second->GetXnObject()->SetActive(false);
    synced_bullets.insert({item.first, item.second});
  }
  unsynced_bullets.clear();
}
