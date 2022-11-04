#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <stack>

#include "Math.h"
#include "RenderComponent.h"
#include "RenderManagerInterface.h"
#include "SingletonBaseClass.h"
#include "stdafx.h"

namespace Xn {

using Microsoft::WRL::ComPtr;

class RenderManager : public RenderManagerInterface {
  friend class RenderComponent;
  friend class Text_RenderComponent;

 public:
  RenderManager(const uint &width, const uint &height,
                const HWND &window_handle);

  // 通过 RenderManagerInterface 继承
  virtual void StartRender() override;
  virtual void EndRender() override;
  virtual void RenderRectSet(const Vector4 &rect) override;
  virtual void RenderRectOver() override;
  virtual void RenderTransformSet(const Vector2 &pos, const Vector2 &scale,
                                  const float &rotation) override;
  virtual void RenderTransformOver() override;
  virtual void OnDestory() override;
  virtual uint GetHeight() override;
  virtual uint GetWidth() override;
  virtual void SetWidthAndHeight(const uint &width,
                                 const uint &height) override;

  void RenderRectangle(const Vector4 &rect, const Vector4 &color);
  void RenderRoundedRectangle(const Vector4 &rect, const float &radius_x,
                              const float &radius_y, const Vector4 &color);
  void RenderEllipse(const Vector2 &pos, const float &radius,
                     const Vector4 &color);
  void RenderEllipse(const Vector2 &pos, const float &radius_x,
                     const float &radius_y, const Vector4 &color);
  void RenderLine(const Vector2 &start_pos, const Vector2 &end_pos,
                  const float &width, const Vector4 &color);
  void RenderText(const Vector2 &start_pos, IDWriteTextLayout *text,
                  const Vector4 &color);

 private:
  uint width_;
  uint height_;

  std::stack<Vector4> render_rect_stack_;
  std::stack<D2D1::Matrix3x2F> transforms_;

  // DXGI
  ComPtr<IDXGIFactory7> dxgi_factory_;
  ComPtr<IDXGIDevice4> dxgi_device_;
  ComPtr<IDXGISwapChain1> swap_chain_;
  ComPtr<IDXGISurface2> dxgi_back_buffer_;
  // D3D
  ComPtr<ID3D11Device> d3d_device_;
  // D2D
  ComPtr<ID2D1Factory7> d2d_factory_;
  ComPtr<ID2D1Device6> d2d_device_;
  ComPtr<ID2D1DeviceContext6> d2d_context_;
  ComPtr<ID2D1Bitmap1> d2d_render_target_bitmap_;
  ComPtr<ID2D1SolidColorBrush> brush_;
  // DWrite
  ComPtr<IDWriteFactory> dwrite_factory_;
  // DComposition
  ComPtr<IDCompositionDevice> dcomp_device_;
  ComPtr<IDCompositionTarget> dcomp_target_;
  ComPtr<IDCompositionVisual> dcomp_visual_;

  CAN_NOT_COPY_THIS_CLASS(RenderManager);
};

}  // namespace Xn
