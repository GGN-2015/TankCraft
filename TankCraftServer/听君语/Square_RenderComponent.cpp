#include "Square_RenderComponent.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "RenderManager.h"
#include "Ìý¾ýÓï.h"

using namespace Xn;

void Xn::Square_RenderComponent::StartRender(const Vector2 &pos,
                                             const Vector2 &scale,
                                             const float &rotation) {
  Ìý¾ýÓï::Get().GetRenderManager()->RenderTransformSet(pos, scale, rotation);
  Ìý¾ýÓï::Get().GetRenderManager()->RenderRectangle(rect_, color_);
}

void Xn::Square_RenderComponent::OverRender() {
  Ìý¾ýÓï::Get().GetRenderManager()->RenderTransformOver();
}
