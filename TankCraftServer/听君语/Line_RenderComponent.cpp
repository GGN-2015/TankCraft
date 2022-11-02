#include "Line_RenderComponent.h"
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "RenderManager.h"
#include "������.h"

void Xn::Line_RenderComponent::StartRender(const Vector2& pos,
                                           const Vector2& scale,
                                           const float& rotation) {
  ������::Get().GetRenderManager()->RenderTransformSet(pos, scale, rotation);
  ������::Get().GetRenderManager()->RenderLine(start_point_, end_point_, width_,
                                               color_);
}

void Xn::Line_RenderComponent::OverRender() {
  ������::Get().GetRenderManager()->RenderTransformOver();
}
