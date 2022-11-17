#include "TankCraft_BulletManager_Component.h"

#include "FileAbout.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "OutputManager.h"
#include "TankCraft_BulletComponent.h"
#include "������.h"

void Xn::TankCraft::BulletManagerComponent::OnStart() {
  ulong _ = 0;
  ReadWavFileIntoMemory(L"�.wav", &audio_bo_, &_);
}

void Xn::TankCraft::BulletManagerComponent::OnDestory() {
  unsynced_bullets_.clear();
  synced_bullets_.clear();

  delete[] audio_bo_;
}

void Xn::TankCraft::BulletManagerComponent::StartSyncBulletState() {
  unsynced_bullets_ = std::move(synced_bullets_);
}

void Xn::TankCraft::BulletManagerComponent::SetBulletState(
    const uint &bullet_id, const Vector2 &pos) {
  // �����ӵ���û��
  auto bullet = unsynced_bullets_.find(bullet_id);
  BulletComponent *new_bullet;
  if (bullet == unsynced_bullets_.end()) {
    // û��userid�ӵ�������һ�����ӵ�
    new_bullet = (BulletComponent *)������::Get()
                     .GetObjectManager()
                     ->CreateXnObject(Vector2::ZERO, GetXnObject())
                     ->AddComponent(std::make_unique<BulletComponent>());
    new_bullet->SetPos(pos);

    ������::Get().GetOutputManager()->PlayAudio(audio_bo_);
  } else {
    // �У��ʹ�δͬ���б�ɾȥ
    new_bullet = bullet->second;
    unsynced_bullets_.erase(bullet);
    if (!new_bullet->GetXnObject()->IsActive()) {
      // δ��ʼ���ӵ�����ʼ��һ��
      new_bullet->GetXnObject()->SetActive(true);
      new_bullet->SetPos(pos);
      ������::Get().GetOutputManager()->PlayAudio(audio_bo_);
    } else
      // �������е��ӵ��������߰�
      new_bullet->SetTargetPos(pos);
  }
  // ���뵽��ͬ���б���
  synced_bullets_.insert({bullet_id, new_bullet});
}

void Xn::TankCraft::BulletManagerComponent::EndSyncBulletState() {
  for (auto &item : unsynced_bullets_) {
    item.second->GetXnObject()->SetActive(false);
    synced_bullets_.insert({item.first, item.second});
  }
  unsynced_bullets_.clear();
}
