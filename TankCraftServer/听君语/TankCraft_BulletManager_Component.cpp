#include "TankCraft_BulletManager_Component.h"

#include "FileAbout.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "OutputManager.h"
#include "TankCraft_BulletComponent.h"
#include "������.h"

Xn::TankCraft::BulletManagerComponent::BulletManagerComponent()
    : Component(L"TanksFight_BulletManagerComponent") {
  audio_bo_ =
      ������::Get().GetOutputManager()->GetOutputManager()->LoadWaveAudio(
          L"�.wav");
  audio_bo_->SetVolume(0);
  audio_bo_->SetCurrentPosition(0);
  audio_bo_->Play(0, 0, 0);
}

void Xn::TankCraft::BulletManagerComponent::OnStart() {}

void Xn::TankCraft::BulletManagerComponent::OnDestory() {
  unsynced_bullets_.clear();
  synced_bullets_.clear();

  audio_bo_.Reset();
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

    audio_bo_->SetVolume(100);
    audio_bo_->SetCurrentPosition(0);
    audio_bo_->Play(0, 0, 0);
  } else {
    // �У��ʹ�δͬ���б�ɾȥ
    new_bullet = bullet->second;
    unsynced_bullets_.erase(bullet);
    if (!new_bullet->GetXnObject()->IsActive()) {
      // δ��ʼ���ӵ�����ʼ��һ��
      new_bullet->GetXnObject()->SetActive(true);
      new_bullet->SetPos(pos);

      audio_bo_->SetVolume(100);
      audio_bo_->SetCurrentPosition(0);
      audio_bo_->Play(0, 0, 0);
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
