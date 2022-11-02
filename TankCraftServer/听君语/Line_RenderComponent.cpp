#include "Line_RenderComponent.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "RenderManager.h"
#include "Ìý¾ýÓï.h"

void Xn::Line_RenderComponent::StartRender(const Vector2& pos,
                                           const Vector2& scale,
                                           const float& rotation) {
  Ìý¾ýÓï::Get().GetRenderManager()->RenderTransformSet(pos, scale, rotation);
  Ìý¾ýÓï::Get().GetRenderManager()->RenderLine(start_point_, end_point_, width_,
                                               color_);
}

void Xn::Line_RenderComponent::OverRender() {
  Ìý¾ýÓï::Get().GetRenderManager()->RenderTransformOver();
}
