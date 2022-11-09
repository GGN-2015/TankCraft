#pragma once

#include <functional>

#include "Component.h"
#include "Math.h"

namespace Xn {

class Text_RenderComponent;
class Square_RenderComponent;

class ButtonComponent : public Component {
 private:
  enum class ButtonState { idel, nopressed, pressed };

 public:
  ButtonComponent(const Vector2& click_area_WH, const Float& font_size,
                  std::function<void()> when_clicked, const Vector4& idle_color,
                  const Vector4& clicked_color)
      : Component(L"ButtonComponent"),
        click_area_WH_(click_area_WH),
        font_size_(font_size),
        when_clicked_(when_clicked),
        idle_color_(idle_color),
        clicked_color_(clicked_color) {}

  virtual void OnStart() override;
  virtual void OnUpdate() override;
  virtual void OnDestory() override;

 public:
  std::wstring GetText();
  void SetText(const std::wstring& text);

 private:
  ButtonState button_state = ButtonState::idel;

  Float color_chang_t_ = 0;
  const Float color_changed_t_ = 0.1f;

  std::wstring text_ = L"°´Å¥";
  Vector2 click_area_WH_;
  Float font_size_;

  Vector4 idle_color_;
  Vector4 clicked_color_;

  std::function<void()> when_clicked_;

  Text_RenderComponent* text_render_component_ = nullptr;
  Square_RenderComponent* background_render_component = nullptr;
};

}  // namespace Xn
