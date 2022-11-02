#include "Transform_RenderComponent.h"
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "RenderManager.h"
#include "������.h"

void Xn::Transform_RenderComponent::StartRender(const Vector2& pos,
                                                const Vector2& scale,
                                                const float& rotation) {
  ������::Get().GetRenderManager()->RenderTransformSet(pos, scale, rotation);
}

void Xn::Transform_RenderComponent::OverRender() {
  ������::Get().GetRenderManager()->RenderTransformOver();
}
