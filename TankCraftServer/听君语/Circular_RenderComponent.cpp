#include "Circular_RenderComponent.h"
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "RenderManager.h"
#include "������.h"

using namespace Xn;

void Circular_RenderComponent::StartRender(const Vector2 &pos,
                                           const Vector2 &scale,
                                           const float &rotation) {
  ������::Get().GetRenderManager()->RenderTransformSet(pos, scale, rotation);
  ������::Get().GetRenderManager()->RenderEllipse(pos, radius_, color_);
}

void Circular_RenderComponent::OverRender() {
  ������::Get().GetRenderManager()->RenderTransformOver();
}
