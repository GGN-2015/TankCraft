#include "ButtonComponent.h"

#include "InputManager.h"
#include "InputTextComponent.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "Square_RenderComponent.h"
#include "Text_RenderComponent.h"
#include "听君语.h"

void Xn::ButtonComponent::OnStart() {
  text_render_component_ =
      (Text_RenderComponent*)听君语::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->SetRenderComponent(std::make_unique<Text_RenderComponent>(
              Vector2::ZERO, text_, Vector2(click_area_WH_.x, click_area_WH_.y),
              click_area_WH_.y * 0.8f));
  text_render_component_->SetColor(Vector4(0.f, 0.f, 0.f, 1));

  background_render_component =
      (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
          std::make_unique<Square_RenderComponent>(idle_color_));
  background_render_component->rect_ = {0, 0, click_area_WH_.x,
                                        click_area_WH_.y};
}

void Xn::ButtonComponent::OnUpdate() {
  switch (button_state) {
    case ButtonState::idel: {
      color_chang_t_ -= 听君语::Get().GetDeltaTime();
      color_chang_t_.SetClamp(0, color_changed_t_);
      background_render_component->color_ =
          Vector4::Lerp(idle_color_, clicked_color_,
                        color_chang_t_.ScaleFromTo(0, color_changed_t_, 0, 1));

      if (听君语::Get().GetInputManager()->mouse_manager_.IsLeftPressed()) {
        // TODO
        // 目前使用最简点击判定，这个组件的物体及其一系列父物体不能有旋转，父物体偏移为0
        auto _click_pos =
            听君语::Get().GetInputManager()->mouse_manager_.GetPos();
        Vector2 click_pos = {_click_pos.first, _click_pos.second};
        if (click_pos.x < GetXnObject()->GetPos().x ||
            click_pos.y < GetXnObject()->GetPos().y ||
            click_pos.x > GetXnObject()->GetPos().x + click_area_WH_.x ||
            click_pos.y > GetXnObject()->GetPos().y + click_area_WH_.y)
          button_state = ButtonState::nopressed;
        else
          button_state = ButtonState::pressed;
      }
    } break;

    case ButtonState::nopressed: {
      color_chang_t_ -= 听君语::Get().GetDeltaTime();
      color_chang_t_.SetClamp(0, color_changed_t_);
      background_render_component->color_ =
          Vector4::Lerp(idle_color_, clicked_color_,
                        color_chang_t_.ScaleFromTo(0, color_changed_t_, 0, 1));

      if (!听君语::Get().GetInputManager()->mouse_manager_.IsLeftPressed())
        button_state = ButtonState::idel;
    } break;

    case ButtonState::pressed: {
      color_chang_t_ += 听君语::Get().GetDeltaTime();
      color_chang_t_.SetClamp(0, color_changed_t_);
      background_render_component->color_ =
          Vector4::Lerp(idle_color_, clicked_color_,
                        color_chang_t_.ScaleFromTo(0, color_changed_t_, 0, 1));

      if (!听君语::Get().GetInputManager()->mouse_manager_.IsLeftPressed()) {
        // TODO
        // 目前使用最简点击判定，这个组件的物体及其一系列父物体不能有旋转，父物体偏移为0
        auto _click_pos =
            听君语::Get().GetInputManager()->mouse_manager_.GetPos();
        Vector2 click_pos = {_click_pos.first, _click_pos.second};
        if (click_pos.x < GetXnObject()->GetPos().x ||
            click_pos.y < GetXnObject()->GetPos().y ||
            click_pos.x > GetXnObject()->GetPos().x + click_area_WH_.x ||
            click_pos.y > GetXnObject()->GetPos().y + click_area_WH_.y)
          button_state = ButtonState::idel;
        else {
          button_state = ButtonState::idel;
          when_clicked_();
        }
      }
    } break;

    default:
      break;
  }
}

void Xn::ButtonComponent::OnDestory() {}

std::wstring Xn::ButtonComponent::GetText() { return text_; }

void Xn::ButtonComponent::SetText(const std::wstring& text) {
  text_ = text;
  text_render_component_->SetText(text_);
}
