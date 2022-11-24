#pragma once

#include <functional>

#include "Component.h"
#include "Math.h"

namespace Xn {

class Text_RenderComponent;
class Square_RenderComponent;

class TextComponent : public Component {
 public:
  TextComponent(const Vector2& text_area_WH, const Float& font_size,
                const Vector4& background_color)
      : Component(L"TextComponent"),
        text_area_WH_(text_area_WH),
        background_color_(background_color),
        font_size_(font_size) {}

  virtual void OnStart() override;
  virtual void OnUpdate() override;
  virtual void OnDestory() override;

 public:
  std::wstring GetText();
  void SetText(const std::wstring& text);

 private:
  std::wstring text_;
  Vector2 text_area_WH_;
  Float font_size_;

  Vector4 background_color_;

  Text_RenderComponent* text_render_component_ = nullptr;
  Square_RenderComponent* background_render_component = nullptr;
};

}  // namespace Xn
