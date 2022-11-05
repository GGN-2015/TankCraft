#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "RenderComponent.h"
#include "stdafx.h"

namespace Xn {

using Microsoft::WRL::ComPtr;

class Text_RenderComponent : public RenderComponent {
 public:
  Text_RenderComponent(const Vector2 &start_pos, const std::wstring &text,
                       const Vector2 &WH, const Float &font_size);

  // ͨ�� RenderComponent �̳�
  virtual void StartRender(const Vector2 &pos, const Vector2 &scale,
                           const float &rotation) override;
  virtual void OverRender() override;

 public:
  void SetText(const std::wstring &text);
  void SetWH(const Vector2 &WH);
  void SetFontSize(const uint &size);
  void SetFontWidth(const DWRITE_FONT_WEIGHT &width);
  void SetColor(const Vector4 &color);

  Vector2 start_pos_;
  Vector4 color_;

 private:
  ComPtr<IDWriteTextLayout> text_;
};

}  // namespace Xn
