#pragma once

#include <array>

#include "Component.h"
#include "Math.h"
#include "typedef.h"

namespace Xn {

class TextComponent;
class Square_RenderComponent;

namespace TankCraft {

class RankingListComponent : public Component {
 public:
  static constexpr uint ranking_list_number_ = 8;
  static const Vector2 ranking_list_start_position_;

 public:
  RankingListComponent();

  virtual void OnStart() override;
  virtual void OnDestory() override;

  void SetThisKillCount(const std::wstring& user_name, const uint& kill_count);
  void SetKillCount(const uint& ranking, const std::wstring& user_name,
                    const uint& kill_count);
  void SetRankingNumber(const uint& number);

 private:
  std::array<TextComponent*, ranking_list_number_ + 1> ranking_list_items_;

  Square_RenderComponent* background_render_component_ = nullptr;
};

}  // namespace TankCraft
}  // namespace Xn
