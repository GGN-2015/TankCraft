#include "Square_RenderComponent.h"
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "RenderManager.h"
#include "������.h"

using namespace Xn;

void Xn::Square_RenderComponent::StartRender(const Vector2 &pos,
                                             const Vector2 &scale,
                                             const float &rotation) {
  ������::Get().GetRenderManager()->RenderTransformSet(pos, scale, rotation);
  ������::Get().GetRenderManager()->RenderRectangle(rect_, color_);
}

void Xn::Square_RenderComponent::OverRender() {
  ������::Get().GetRenderManager()->RenderTransformOver();
}
