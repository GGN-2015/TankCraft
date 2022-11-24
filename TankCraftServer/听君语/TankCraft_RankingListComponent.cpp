#include "TankCraft_RankingListComponent.h"

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "TextComponent.h"
#include "Ìý¾ýÓï.h"

const Xn::Vector2
    Xn::TankCraft::RankingListComponent::ranking_list_start_position_(30, 400);

Xn::TankCraft::RankingListComponent::RankingListComponent()
    : Component(L"TankCraft_RankingListComponent"), ranking_list_items_{} {
  for (uint i = 0; i <= ranking_list_number_; ++i) {
    ranking_list_items_[i] =
        (TextComponent*)Ìý¾ýÓï::Get()
            .GetObjectManager()
            ->CreateXnObject(ranking_list_start_position_ + Vector2(0, i * 30))
            ->AddComponent(std::make_unique<TextComponent>(
                Vector2(100, 28), 24.f,
                Vector4::Color::RGB255(187, 173, 243, 200)));
  }
}

void Xn::TankCraft::RankingListComponent::OnStart() {
  for (auto item : ranking_list_items_) item->SetText(L"");
}

void Xn::TankCraft::RankingListComponent::OnDestory() {}

void Xn::TankCraft::RankingListComponent::SetThisKillCount(
    const std::wstring& user_name, const uint& kill_count) {

  if (user_name.size() > 3)
    ranking_list_items_[ranking_list_number_]->SetText(
        user_name.substr(0, 3) + L":" + std::to_wstring(kill_count));
  else
    ranking_list_items_[ranking_list_number_]->SetText(
        user_name + L":" + std::to_wstring(kill_count));
}

void Xn::TankCraft::RankingListComponent::SetKillCount(
    const uint& ranking, const std::wstring& user_name,
    const uint& kill_count) {
  if (ranking >= ranking_list_number_) return;

  if (user_name.size() > 3)
    ranking_list_items_[ranking]->SetText(user_name.substr(0, 3) + L":" +
                                          std::to_wstring(kill_count));
  else
    ranking_list_items_[ranking]->SetText(user_name + L":" +
                                          std::to_wstring(kill_count));
}

void Xn::TankCraft::RankingListComponent::SetRankingNumber(const uint& number) {
  for (uint i = number; i < ranking_list_number_; ++i)
    ranking_list_items_[i]->SetText(L"");
}
