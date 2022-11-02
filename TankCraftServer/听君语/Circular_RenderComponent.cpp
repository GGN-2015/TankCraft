#include "Circular_RenderComponent.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "RenderManager.h"
#include "Ìý¾ýÓï.h"

using namespace Xn;

void Circular_RenderComponent::StartRender(const Vector2 &pos,
                                           const Vector2 &scale,
                                           const float &rotation) {
  Ìý¾ýÓï::Get().GetRenderManager()->RenderTransformSet(pos, scale, rotation);
  Ìý¾ýÓï::Get().GetRenderManager()->RenderEllipse(pos, radius_, color_);
}

void Circular_RenderComponent::OverRender() {
  Ìý¾ýÓï::Get().GetRenderManager()->RenderTransformOver();
}
