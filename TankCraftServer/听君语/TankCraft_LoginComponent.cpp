#include "TankCraft_LoginComponent.h"

#include "InputManager.h"
#include "ObjectManager-XnObject.h"
#include "Text_RenderComponent.h"
#include "Ìý¾ýÓï.h"

void Xn::TankCraft::LoginComponent::OnStart() {
  render_component_ = (Text_RenderComponent*)GetXnObject()->SetRenderComponent(
      std::make_unique<Text_RenderComponent>(
          Vector2::ZERO, L"ÇëÊäÈëÓÃ»§Ãû(»Ø³µµÇÂ¼)", Vector2(500, 50), 48.f));

  render_component_->SetColor(Vector4(0.f, 0.f, 0.f, 1));
}

void Xn::TankCraft::LoginComponent::OnUpdate() {
  auto c = Ìý¾ýÓï::Get().GetInputManager()->keyboard_manager_.ReadChar();
  if (c) {
    if (c == L'\r') {
      text_.clear();
      render_component_->SetText(L"µÇÂ¼");
    } else {
      text_.push_back(c);
      render_component_->SetText(text_);
    }
  }
}

void Xn::TankCraft::LoginComponent::OnDestory() { text_ = L""; }
