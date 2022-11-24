#include "TextComponent.h"

#include "InputManager.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "Square_RenderComponent.h"
#include "Text_RenderComponent.h"
#include "Ìý¾ýÓï.h"

void Xn::TextComponent::OnStart() {
  text_render_component_ =
      (Text_RenderComponent*)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->SetRenderComponent(std::make_unique<Text_RenderComponent>(
              Vector2::ZERO, text_, text_area_WH_, font_size_));
  text_render_component_->SetColor(Vector4(0.f, 0.f, 0.f, 1));

  background_render_component =
      (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
          std::make_unique<Square_RenderComponent>(background_color_));
  background_render_component->rect_ = {0, 0, text_area_WH_.x, text_area_WH_.y};
}

void Xn::TextComponent::OnUpdate() {}

void Xn::TextComponent::OnDestory() {}

std::wstring Xn::TextComponent::GetText() { return text_; }

void Xn::TextComponent::SetText(const std::wstring& text) {
  text_ = text;
  text_render_component_->SetText(text_);
}
