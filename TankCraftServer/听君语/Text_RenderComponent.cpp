#include "Text_RenderComponent.h"

#include "ForDebug.h"
#include "RenderManager.h"
#include "Ìý¾ýÓï.h"

Xn::Text_RenderComponent::Text_RenderComponent(const Vector2& start_pos,
                                               const std::wstring& text,
                                               const Vector2& WH,
                                               const Float& font_size)
    : RenderComponent(), start_pos_(start_pos), color_(Vector4::ZERO) {
  auto& dwrite_factory = Ìý¾ýÓï::Get().GetRenderManager()->dwrite_factory_;
  ComPtr<IDWriteTextFormat> text_format;
  XN_THROW_IF_FAILED(dwrite_factory->CreateTextFormat(
      L"Microsoft Yahei", nullptr, DWRITE_FONT_WEIGHT_REGULAR,
      DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, font_size, L"zh-cn",
      &text_format));

  Ìý¾ýÓï::Get().GetRenderManager()->dwrite_factory_->CreateTextLayout(
      text.data(), text.size(), text_format.Get(), WH.asWH.width,
      WH.asWH.height, &text_);
}

void Xn::Text_RenderComponent::StartRender(const Vector2& pos,
                                           const Vector2& scale,
                                           const float& rotation) {
  Ìý¾ýÓï::Get().GetRenderManager()->RenderTransformSet(pos, scale, rotation);
  Ìý¾ýÓï::Get().GetRenderManager()->RenderText(start_pos_, text_.Get(), color_);
}

void Xn::Text_RenderComponent::OverRender() {
  Ìý¾ýÓï::Get().GetRenderManager()->RenderTransformOver();
}
