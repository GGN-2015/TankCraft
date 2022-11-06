#include "InputTextComponent.h"

#include "InputManager.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "Square_RenderComponent.h"
#include "Text_RenderComponent.h"
#include "������.h"

void Xn::InputTextComponent::OnStart() {
  text_render_component_ =
      (Text_RenderComponent*)������::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->SetRenderComponent(std::make_unique<Text_RenderComponent>(
              Vector2::ZERO, default_text_,
              Vector2(input_area_WH_.x, input_area_WH_.y),
              input_area_WH_.y * 0.8f));
  text_render_component_->SetColor(Vector4(0.f, 0.f, 0.f, 1));

  background_render_component =
      (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
          std::make_unique<Square_RenderComponent>(idle_color_));
  background_render_component->rect_ = {0, 0, input_area_WH_.x,
                                        input_area_WH_.y};
}

void Xn::InputTextComponent::OnUpdate() {
  if (!is_inputing_) {
    color_chang_t_ -= ������::Get().GetDeltaTime();
    color_chang_t_.SetClamp(0, color_changed_t_);
    background_render_component->color_ =
        Vector4::Lerp(idle_color_, inputing_color_,
                      color_chang_t_.ScaleFromTo(0, color_changed_t_, 0, 1));

    if (!������::Get().GetInputManager()->mouse_manager_.IsLeftPressed())
      return;
    // TODO
    // Ŀǰʹ��������ж��������������弰��һϵ�и����岻������ת��������ƫ��Ϊ0
    auto _click_pos = ������::Get().GetInputManager()->mouse_manager_.GetPos();
    Vector2 click_pos = {_click_pos.first, _click_pos.second};
    if (click_pos.x < GetXnObject()->GetPos().x ||
        click_pos.y < GetXnObject()->GetPos().y ||
        click_pos.x > GetXnObject()->GetPos().x + input_area_WH_.x ||
        click_pos.y > GetXnObject()->GetPos().y + input_area_WH_.y)
      return;
    StartInput();
    return;
  }

  color_chang_t_ += ������::Get().GetDeltaTime();
  color_chang_t_.SetClamp(0, color_changed_t_);
  background_render_component->color_ =
      Vector4::Lerp(idle_color_, inputing_color_,
                    color_chang_t_.ScaleFromTo(0, color_changed_t_, 0, 1));

  if (������::Get().GetInputManager()->mouse_manager_.IsLeftPressed()) {
    // TODO
    // Ŀǰʹ��������ж��������������弰��һϵ�и����岻������ת��������ƫ��Ϊ0
    auto _click_pos = ������::Get().GetInputManager()->mouse_manager_.GetPos();
    Vector2 click_pos = {_click_pos.first, _click_pos.second};
    if (click_pos.x < GetXnObject()->GetPos().x ||
        click_pos.y < GetXnObject()->GetPos().y ||
        click_pos.x > GetXnObject()->GetPos().x + input_area_WH_.x ||
        click_pos.y > GetXnObject()->GetPos().y + input_area_WH_.y) {
      EndInput();
      return;
    }
  }

  auto c = ������::Get().GetInputManager()->keyboard_manager_.ReadChar();
  if (c) {
    if (c == L'\r') {
      EndInput();
    } else if (c == L'\b') {
      if (text_.size() >= 1) {
        text_.pop_back();
        CheckShowText();
      }
    } else {
      text_.push_back(c);
      CheckShowText();
    }
  }
}

void Xn::InputTextComponent::OnDestory() {}

std::wstring Xn::InputTextComponent::GetText() { return text_; }

void Xn::InputTextComponent::SetText(const std::wstring& text) {
  text_ = text;
  CheckShowText();
}

void Xn::InputTextComponent::SetDefaultShowText(const std::wstring& text) {
  default_text_ = text;
  CheckShowText();
}

void Xn::InputTextComponent::Clear() {
  text_.clear();
  CheckShowText();
}

void Xn::InputTextComponent::StartInput() {
  is_inputing_ = true;
  ������::Get().GetInputManager()->keyboard_manager_.FlushChar();
}

void Xn::InputTextComponent::EndInput() {
  is_inputing_ = false;
  when_end_input_();
}

void Xn::InputTextComponent::CheckShowText() {
  if (text_.size())
    text_render_component_->SetText(text_);
  else
    text_render_component_->SetText(default_text_);
}
