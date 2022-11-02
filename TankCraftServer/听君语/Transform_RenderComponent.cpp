#include "Transform_RenderComponent.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "RenderManager.h"
#include "Ìý¾ýÓï.h"

void Xn::Transform_RenderComponent::StartRender(const Vector2& pos,
                                                const Vector2& scale,
                                                const float& rotation) {
  Ìý¾ýÓï::Get().GetRenderManager()->RenderTransformSet(pos, scale, rotation);
}

void Xn::Transform_RenderComponent::OverRender() {
  Ìý¾ýÓï::Get().GetRenderManager()->RenderTransformOver();
}
