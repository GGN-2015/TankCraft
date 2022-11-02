#include "RenderManager.h"
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "FileAbout.h"
#include "ForDebug.h"
#include "ObjectManager-XnObject.h"
#include "RenderComponent.h"
#include "stdafx.h"
#include "听君语.h"

using namespace Xn;

RenderManager::RenderManager(const uint &width, const uint &height,
                             const HWND &window_handle)
    : width_(width), height_(height) {
  uint creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
  D2D1_FACTORY_OPTIONS d2d_factory_options = {};
#ifdef _DEBUG
  creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
  d2d_factory_options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
  d2d_factory_options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif  // _DEBUG

  // 创建direct3D设备
  XN_THROW_IF_FAILED(D3D11CreateDevice(
      nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, creation_flags, nullptr, 0,
      D3D11_SDK_VERSION, &d3d_device_, nullptr, nullptr));

  // 创建dxgi设备
  XN_THROW_IF_FAILED(d3d_device_.As(&dxgi_device_));

  // 创建dxgi工厂
  {
    // 创建dxgi适配器
    ComPtr<IDXGIAdapter> dxgi_adapter;
    XN_THROW_IF_FAILED(dxgi_device_->GetAdapter(&dxgi_adapter));

    XN_THROW_IF_FAILED(dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory_)));
  }

  // 创建direct2D工厂
  XN_THROW_IF_FAILED(D2D1CreateFactory<ID2D1Factory7>(
      D2D1_FACTORY_TYPE_SINGLE_THREADED, d2d_factory_options, &d2d_factory_));

  // 创建direct2D设备
  XN_THROW_IF_FAILED(
      d2d_factory_->CreateDevice(dxgi_device_.Get(), &d2d_device_));

  // 创建direct2D上下文
  XN_THROW_IF_FAILED(d2d_device_->CreateDeviceContext(
      D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2d_context_));

  // 创建direct write工厂
  XN_THROW_IF_FAILED(DWriteCreateFactory(
      DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
      reinterpret_cast<IUnknown **>(dwrite_factory_.GetAddressOf())));

  // 创建direct composition设备
  XN_THROW_IF_FAILED(DCompositionCreateDevice(dxgi_device_.Get(),
                                              IID_PPV_ARGS(&dcomp_device_)));

  // 创建direct composition目标
  XN_THROW_IF_FAILED(
      dcomp_device_->CreateTargetForHwnd(window_handle, false, &dcomp_target_));

  // 创建direct composition视觉对象
  XN_THROW_IF_FAILED(dcomp_device_->CreateVisual(&dcomp_visual_));

  // 创建交换链
  {
    // 设置交换链描述
    DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {
        .Width = width,
        .Height = height,
        .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
        .Stereo = false,
        .SampleDesc = {.Count = 1,  // 多线程渲染才需要填超过1的值
                       .Quality = 0},
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 2,
        .Scaling = DXGI_SCALING_STRETCH,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
        .AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED,
        .Flags = 0,
    };
    XN_THROW_IF_FAILED(dxgi_factory_->CreateSwapChainForComposition(
        d3d_device_.Get(), &swap_chain_desc, nullptr, &swap_chain_));
  }

  // 限制一次最大渲染帧数
  XN_THROW_IF_FAILED(dxgi_device_->SetMaximumFrameLatency(1));

  // 设置direct2D渲染目标
  {
    // 取得dxgi版本的后缓冲的表面指针
    XN_THROW_IF_FAILED(
        swap_chain_->GetBuffer(0, IID_PPV_ARGS(&dxgi_back_buffer_)));

    // 发动【链接：D2D渲染位图 与 交换链】的参数
    D2D1_BITMAP_PROPERTIES1 bitmap_properties = {
        .pixelFormat = {.format = DXGI_FORMAT_B8G8R8A8_UNORM,
                        .alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED},
        .bitmapOptions =
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW};

    // 创建direct2D渲染目标
    XN_THROW_IF_FAILED(d2d_context_->CreateBitmapFromDxgiSurface(
        dxgi_back_buffer_.Get(), &bitmap_properties,
        &d2d_render_target_bitmap_));

    d2d_context_->SetTarget(d2d_render_target_bitmap_.Get());
  }

  // 设置direct composition渲染目标
  dcomp_visual_->SetContent(swap_chain_.Get());
  dcomp_target_->SetRoot(dcomp_visual_.Get());

  // 创建direct2D笔刷
  XN_THROW_IF_FAILED(d2d_context_->CreateSolidColorBrush(
      D2D1::ColorF(D2D1::ColorF::Pink), &brush_));

  Vector4 client_rect = {0, 0, width_, height_};
  render_rect_stack_.push(client_rect);

  D2D1::Matrix3x2F d2d_start_transform_ = D2D1::Matrix3x2F::Identity();
  transforms_.push(d2d_start_transform_);
}

void Xn::RenderManager::StartRender() {
  d2d_context_->BeginDraw();
  d2d_context_->Clear(D2D1::ColorF(D2D1::ColorF::White, 0.2f));
}

void Xn::RenderManager::EndRender() {
  XN_THROW_IF_FAILED(d2d_context_->EndDraw());

  XN_THROW_IF_FAILED(swap_chain_->Present(1, 0));

  XN_THROW_IF_FAILED(dcomp_device_->Commit());
}

void Xn::RenderManager::RenderRectSet(const Vector4 &rect) {
  render_rect_stack_.push(rect);  // TODO
}

void Xn::RenderManager::RenderRectOver() {
  render_rect_stack_.pop();
  // TODO
}

void Xn::RenderManager::RenderTransformSet(const Vector2 &pos,
                                           const Vector2 &scale,
                                           const float &rotation) {
  const auto &now_transform = transforms_.top();

  auto the_transform = D2D1::Matrix3x2F::Scale(scale.x, scale.y) *
                       D2D1::Matrix3x2F::Rotation(rotation) *
                       D2D1::Matrix3x2F::Translation(pos.x, pos.y);
  the_transform = the_transform * now_transform;
  transforms_.push(the_transform);
  d2d_context_->SetTransform(the_transform);
}

void Xn::RenderManager::RenderTransformOver() {
  transforms_.pop();
  auto &the_transform = transforms_.top();
  d2d_context_->SetTransform(the_transform);
}

void RenderManager::OnDestory() {
  d2d_factory_->Release();
  d2d_device_->Release();
  d2d_context_->Release();
  d2d_render_target_bitmap_->Release();
  brush_->Release();
}

uint Xn::RenderManager::GetHeight() { return height_; }

uint Xn::RenderManager::GetWidth() { return width_; }

void Xn::RenderManager::SetWidthAndHeight(const uint &width,
                                          const uint &height) {
  width_ = width;
  height_ = height;
  render_rect_stack_.pop();
  Vector4 client_rect(0, 0, width, height);
  render_rect_stack_.push(client_rect);

  d2d_context_.Reset();
  d2d_render_target_bitmap_.Reset();
  dxgi_back_buffer_.Reset();

  XN_THROW_IF_FAILED(
      swap_chain_->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0));

  // 创建direct2D上下文
  XN_THROW_IF_FAILED(d2d_device_->CreateDeviceContext(
      D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2d_context_));

  // 设置direct2D渲染目标
  {
    // 取得dxgi版本的后缓冲的表面指针
    XN_THROW_IF_FAILED(
        swap_chain_->GetBuffer(0, IID_PPV_ARGS(&dxgi_back_buffer_)));

    // 发动【链接：D2D渲染位图 与 交换链】的参数
    D2D1_BITMAP_PROPERTIES1 bitmap_properties = {
        .pixelFormat = {.format = DXGI_FORMAT_B8G8R8A8_UNORM,
                        .alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED},
        .bitmapOptions =
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW};

    // 创建direct2D渲染目标
    XN_THROW_IF_FAILED(d2d_context_->CreateBitmapFromDxgiSurface(
        dxgi_back_buffer_.Get(), &bitmap_properties,
        &d2d_render_target_bitmap_));

    d2d_context_->SetTarget(d2d_render_target_bitmap_.Get());
  }

  // 设置direct composition渲染目标
  dcomp_visual_->SetContent(swap_chain_.Get());
  dcomp_target_->SetRoot(dcomp_visual_.Get());
}
void Xn::RenderManager::RenderRectangle(const Vector4 &rect,
                                        const Vector4 &color) {
  brush_->SetColor(D2D1::ColorF(color.asColor.R, color.asColor.G,
                                color.asColor.B, color.asColor.A));
  D2D1_RECT_F d2d_rect = {.left = rect.asRect.left,
                          .top = rect.asRect.top,
                          .right = rect.asRect.right,
                          .bottom = rect.asRect.bottom};
  d2d_context_->FillRectangle(d2d_rect, brush_.Get());
}
void Xn::RenderManager::RenderRoundedRectangle(const Vector4 &rect,
                                               const float &radius_x,
                                               const float &radius_y,
                                               const Vector4 &color) {
  brush_->SetColor(D2D1::ColorF(color.asColor.R, color.asColor.G,
                                color.asColor.B, color.asColor.A));
  D2D1_ROUNDED_RECT d2d_rounded_rect = {.rect = {.left = rect.asRect.left,
                                                 .top = rect.asRect.top,
                                                 .right = rect.asRect.right,
                                                 .bottom = rect.asRect.bottom},
                                        .radiusX = radius_x,
                                        .radiusY = radius_y};
  d2d_context_->FillRoundedRectangle(&d2d_rounded_rect, brush_.Get());
}
void Xn::RenderManager::RenderEllipse(const Vector2 &pos, const float &radius,
                                      const Vector4 &color) {
  brush_->SetColor(D2D1::ColorF(color.asColor.R, color.asColor.G,
                                color.asColor.B, color.asColor.A));
  D2D1_ELLIPSE d2d_ellipse = {
      .point = {.x = pos.x, .y = pos.y}, .radiusX = radius, .radiusY = radius};
  d2d_context_->FillEllipse(d2d_ellipse, brush_.Get());
}
void Xn::RenderManager::RenderEllipse(const Vector2 &pos, const float &radius_x,
                                      const float &radius_y,
                                      const Vector4 &color) {
  brush_->SetColor(D2D1::ColorF(color.asColor.R, color.asColor.G,
                                color.asColor.B, color.asColor.A));
  D2D1_ELLIPSE d2d_ellipse = {.point = {.x = pos.x, .y = pos.y},
                              .radiusX = radius_x,
                              .radiusY = radius_y};
  d2d_context_->FillEllipse(d2d_ellipse, brush_.Get());
}
void Xn::RenderManager::RenderLine(const Vector2 &start_pos,
                                   const Vector2 &end_pos, const float &width,
                                   const Vector4 &color) {
  brush_->SetColor(D2D1::ColorF(color.asColor.R, color.asColor.G,
                                color.asColor.B, color.asColor.A));
  d2d_context_->DrawLine(D2D1::Point2F(start_pos.x, start_pos.y),
                         D2D1::Point2F(end_pos.x, end_pos.y), brush_.Get(),
                         width);
}
