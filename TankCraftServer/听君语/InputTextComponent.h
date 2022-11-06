#pragma once

#include <functional>

#include "Component.h"
#include "Math.h"

namespace Xn {

class Text_RenderComponent;
class Square_RenderComponent;

class InputTextComponent : public Component {
 public:
  InputTextComponent(const Vector2& input_area_WH,
                     std::function<void()> when_end_input,
                     const Vector4& idle_color, const Vector4& inputing_color)
      : Component(L"InputTextComponent"),
        input_area_WH_(input_area_WH),
        when_end_input_(when_end_input),
        idle_color_(idle_color),
        inputing_color_(inputing_color) {}

  virtual void OnStart() override;
  virtual void OnUpdate() override;
  virtual void OnDestory() override;

 public:
  std::wstring GetText();
  void SetText(const std::wstring& text);
  void SetDefaultShowText(const std::wstring& text);
  void Clear();
  void StartInput();
  void EndInput();
  void CheckShowText();

 private:
  bool is_inputing_ = false;

  Float color_chang_t_ = 0;
  const Float color_changed_t_ = 0.1f;

  std::wstring text_;
  std::wstring default_text_ = L"«Î ‰»Î";
  Vector2 input_area_WH_;

  Vector4 idle_color_;
  Vector4 inputing_color_;

  std::function<void()> when_end_input_;

  Text_RenderComponent* text_render_component_ = nullptr;
  Square_RenderComponent* background_render_component = nullptr;
};

}  // namespace Xn
