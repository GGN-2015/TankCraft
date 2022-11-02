#include "TankCraft_TankComponent.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "Circular_RenderComponent.h"
#include "ObjectManager-XnObject.h"

using namespace Xn;
using namespace TankCraft;

void TankComponent::OnStart() {
  render_component_ =
      (Circular_RenderComponent *)GetXnObject()->SetRenderComponent(
          std::make_unique<Circular_RenderComponent>());

#if _DEBUG && 1
  SetPos(Vector2::Random({0, 0}, {10, 10}),
         Vector2::Random(Vector2::X, Vector2::Y));
  SetColor(Vector3::Random(Vector3::ZERO, Vector3::ONE));
#endif  // Test
}
void TankComponent::OnUpdate() {}
void TankComponent::OnDestory() {}

void TankComponent::SetLerpTime(const Float &lerp_time) {}

void TankComponent::SetPos(const Vector2 &pos, const Float &rotation) {}
void TankComponent::SetPos(const Vector2 &pos, const Vector2 &direction) {}
void TankComponent::SetTargetPos(const Vector2 &pos) {}
void TankComponent::SetRadio(const Float &radio) {}
void TankComponent::SetColor(const Vector3 &color) {}
void TankComponent::SetColor(const Vector4 &color) {}

void TankComponent::BindUser() {}
