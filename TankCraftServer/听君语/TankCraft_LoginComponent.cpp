#include "TankCraft_LoginComponent.h"

#include "ButtonComponent.h"
#include "InputManager.h"
#include "InputTextComponent.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "Square_RenderComponent.h"
#include "TankCraft_GameManager_Component.h"
#include "Ìý¾ýÓï.h"

void Xn::TankCraft::LoginComponent::OnStart() {
  background_render_component =
      (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
          std::make_unique<Square_RenderComponent>(Vector4(0.9f, 0, 0, 1)));

  name_input_layout =
      (InputTextComponent*)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->AddComponent(std::make_unique<InputTextComponent>(
              Vector2(400, 30),
              [this]() {
                auto port_text = name_input_layout->GetText();
                std::wstring new_port_text = L"";
                for (auto c : port_text) {
                  new_port_text.push_back(c);
                  if (new_port_text.size() >= 16) break;
                }
                name_input_layout->SetText(new_port_text);
              },
              Vector4(191 / 255.f, 255 / 255.f, 220 / 255.f, 0.7f),
              Vector4(200 / 255.f, 200 / 255.f, 225 / 255.f, 1.f)));
  name_input_layout->GetXnObject()->pos_ = {100, 100};
  name_input_layout->SetDefaultShowText(L"ÊäÈëÄãµÄÃû×Ö");
  ipv4_input_layout =
      (InputTextComponent*)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->AddComponent(std::make_unique<InputTextComponent>(
              Vector2(300, 30),
              [this]() {
                auto port_text = ipv4_input_layout->GetText();
                std::wstring new_port_text = L"";
                for (auto c : port_text) {
                  if ((c >= L'0' && c <= L'9') || c == L'.') {
                    new_port_text.push_back(c);
                    if (new_port_text.size() >= 15) break;
                  }
                }
                ipv4_input_layout->SetText(new_port_text);
              },
              Vector4(191 / 255.f, 255 / 255.f, 220 / 255.f, 0.7f),
              Vector4(200 / 255.f, 200 / 255.f, 225 / 255.f, 1.f)));
  ipv4_input_layout->GetXnObject()->pos_ = {100, 200};
  ipv4_input_layout->SetDefaultShowText(L"ÊäÈë·þÎñÆ÷IP£¨IPV4£©");
  port_input_layout =
      (InputTextComponent*)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->AddComponent(std::make_unique<InputTextComponent>(
              Vector2(100, 30),
              [this]() {
                auto port_text = port_input_layout->GetText();
                std::wstring new_port_text = L"";
                for (auto c : port_text) {
                  if (c >= L'0' && c <= L'9') {
                    new_port_text.push_back(c);
                    if (new_port_text.size() >= 5) break;
                  }
                }
                port_input_layout->SetText(new_port_text);
              },
              Vector4(191 / 255.f, 255 / 255.f, 220 / 255.f, 0.8f),
              Vector4(200 / 255.f, 200 / 255.f, 225 / 255.f, 1.f)));
  port_input_layout->GetXnObject()->pos_ = {400, 200};
  port_input_layout->SetDefaultShowText(L"ÊäÈë·þÎñÆ÷Port");

  login_button =
      (ButtonComponent*)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->AddComponent(std::make_unique<ButtonComponent>(
              Vector2(100, 50),
              [this]() {
                game_manager_->ConnectToServer(ipv4_input_layout->GetText(),
                                               port_input_layout->GetText());
              },
              Vector4(255 / 255.f, 255 / 255.f, 220 / 255.f, 0.7f),
              Vector4(200 / 255.f, 200 / 255.f, 225 / 255.f, 1.f)));
  login_button->GetXnObject()->pos_ = {500, 200};
}

void Xn::TankCraft::LoginComponent::OnUpdate() {}

void Xn::TankCraft::LoginComponent::OnDestory() {}
