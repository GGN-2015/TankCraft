#include "Text_RenderComponent.h"

#include "ForDebug.h"
#include "RenderManager.h"
#include "������.h"

Xn::Text_RenderComponent::Text_RenderComponent(const Vector2& start_pos,
                                               const std::wstring& text,
                                               const Vector2& WH,
                                               const Float& font_size)
    : RenderComponent(), start_pos_(start_pos), color_(Vector4::ZERO) {
  auto& dwrite_factory = ������::Get().GetRenderManager()->dwrite_factory_;
  ComPtr<IDWriteTextFormat> text_format;
  XN_THROW_IF_FAILED(dwrite_factory->CreateTextFormat(
      L"Microsoft Yahei", nullptr, DWRITE_FONT_WEIGHT_REGULAR,
      DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, font_size, L"zh-cn",
      &text_format));

  ������::Get().GetRenderManager()->dwrite_factory_->CreateTextLayout(
      text.data(), text.size(), text_format.Get(), WH.asWH.width,
      WH.asWH.height, &text_);
}

void Xn::Text_RenderComponent::StartRender(const Vector2& pos,
                                           const Vector2& scale,
                                           const float& rotation) {
  ������::Get().GetRenderManager()->RenderTransformSet(pos, scale, rotation);
  ������::Get().GetRenderManager()->RenderText(start_pos_, text_.Get(), color_);
}
void Xn::Text_RenderComponent::OverRender() {
  ������::Get().GetRenderManager()->RenderTransformOver();
}

void Xn::Text_RenderComponent::SetText(const std::wstring& text) {
  auto& dwrite_factory = ������::Get().GetRenderManager()->dwrite_factory_;
  ComPtr<IDWriteTextFormat> text_format;
  XN_THROW_IF_FAILED(dwrite_factory->CreateTextFormat(
      L"Microsoft Yahei", nullptr, DWRITE_FONT_WEIGHT_REGULAR,
      DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
      text_->GetFontSize(), L"zh-cn", &text_format));

  ������::Get().GetRenderManager()->dwrite_factory_->CreateTextLayout(
      text.data(), text.size(), text_format.Get(), text_->GetMaxWidth(),
      text_->GetMaxHeight(), &text_);
}
void Xn::Text_RenderComponent::SetWH(const Vector2& WH) {
  text_->SetMaxWidth(WH.asWH.width);
  text_->SetMaxHeight(WH.asWH.height);
}
void Xn::Text_RenderComponent::SetFontSize(const uint& size) {
  DWRITE_TEXT_RANGE text_range = {.startPosition = 0, .length = 256};
  text_->SetFontSize(size, text_range);
}
void Xn::Text_RenderComponent::SetFontWidth(const DWRITE_FONT_WEIGHT& width) {
  DWRITE_TEXT_RANGE text_range = {.startPosition = 0, .length = 256};
  text_->SetFontWeight(width, text_range);
}
void Xn::Text_RenderComponent::SetColor(const Vector4& color) {
  color_ = color;
}