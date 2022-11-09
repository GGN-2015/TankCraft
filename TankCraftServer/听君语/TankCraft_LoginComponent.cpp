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
          std::make_unique<Square_RenderComponent>(
              Vector4(255 / 255.f, 122 / 255.f, 164 / 255.f, 1)));
  background_render_component->rect_ = {10, 10, 190, 220};

  ipv4_input_layout =
      (InputTextComponent*)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2(18, 25), GetXnObject())
          ->AddComponent(std::make_unique<InputTextComponent>(
              Vector2(164, 30), 18,
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
  ipv4_input_layout->SetDefaultShowText(L"·þÎñÆ÷IP");
  port_input_layout =
      (InputTextComponent*)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2(70, 60), GetXnObject())
          ->AddComponent(std::make_unique<InputTextComponent>(
              Vector2(60, 20), 17,
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
  port_input_layout->SetDefaultShowText(L"¶Ë¿ÚºÅ");

  connect_button =
      (ButtonComponent*)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2(25, 85), GetXnObject())
          ->AddComponent(std::make_unique<ButtonComponent>(
              Vector2(150, 26), 22,
              [this]() {
                game_manager_->ConnectToServer(ipv4_input_layout->GetText(),
                                               port_input_layout->GetText());
              },
              Vector4(255 / 255.f, 255 / 255.f, 220 / 255.f, 0.7f),
              Vector4(200 / 255.f, 200 / 255.f, 225 / 255.f, 1.f)));
  connect_button->SetText(L"Á¬½Ó·þÎñÆ÷");

  name_input_layout =
      (InputTextComponent*)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2(15, 120), GetXnObject())
          ->AddComponent(std::make_unique<InputTextComponent>(
              Vector2(170, 65), 20,
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
  name_input_layout->SetDefaultShowText(L"Ì¹¿ËÃû");

  login_button =
      (ButtonComponent*)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2(25, 190), GetXnObject())
          ->AddComponent(std::make_unique<ButtonComponent>(
              Vector2(150, 26), 22,
              [this]() { game_manager_->Login(name_input_layout->GetText()); },
              Vector4(255 / 255.f, 255 / 255.f, 220 / 255.f, 0.7f),
              Vector4(200 / 255.f, 200 / 255.f, 225 / 255.f, 1.f)));
  login_button->SetText(L"µÇÂ¼");
}

void Xn::TankCraft::LoginComponent::OnUpdate() {}

void Xn::TankCraft::LoginComponent::OnDestory() {}
