#include "������.h"
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "ForDebug.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "PlatformManager.h"
#include "RenderManager.h"
#include "TestComponent.h"
#include "stdafx.h"

using namespace Xn;

void ������::Init(const HINSTANCE &instance_handle_, const uint &width,
                  const uint &height) {
  XN_THROW_IF_FAILED(DXGIDeclareAdapterRemovalSupport());

  platform_manager_ = std::make_unique<PlatformManager>(
      instance_handle_, L"������", width, height);
  PlatformManager *platform_manager =
      (PlatformManager *)platform_manager_.get();
  render_manager_ = std::make_unique<RenderManager>(
      width, height, platform_manager->GetWindowHandle());
  object_manager_ = std::make_unique<ObjectManager>();
  input_manager_ = std::make_unique<InputManager>();

  time_.NextFrame();
}

int ������::Run() {
  try {
    ������::Get().Start();
    PlatformManager *platform_manager =
        (PlatformManager *)platform_manager_.get();
    return platform_manager->Run();
  } catch (const XnException &e) {
    MessageBox(nullptr, e.What(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
  } catch (const std::exception &e) {
    MessageBoxA(nullptr, e.what(), "StdException", MB_OK | MB_ICONEXCLAMATION);
  } catch (...) {
    MessageBox(nullptr, L"No details available", L"Unknow Exception",
               MB_OK | MB_ICONEXCLAMATION);
  }
  return -1;
}

void Xn::������::Start() {
  auto object = object_manager_->CreateXnObject();
  OutputDebugString((L"������һ�����壺" + object->GetName() + L"\n").c_str());

  object->AddComponent(std::make_unique<TestComponent>());

  auto object2 = object_manager_->CreateXnObject();
  OutputDebugString((L"������һ�����壺" + object2->GetName() + L"\n").c_str());

  object2->AddComponent(std::make_unique<TestComponent>());

  object_manager_->OnStart();
}

void ������::Update() {
  time_.NextFrame();
  object_manager_->OnUpdate();
}

void ������::Render() {
  render_manager_->StartRender();
  object_manager_->GetRootObject()->OnRender();
  render_manager_->EndRender();
}

void ������::OnSizeChanged(uint width, uint height, bool minimized) {
  render_manager_->SetWidthAndHeight(width, height);
}

void ������::OnWindowMoved(int, int) {}

void ������::OnDestroy() { render_manager_->OnDestory(); }

float Xn::������::GetDeltaTime() { return time_.GetDeltaTime(); }

float Xn::������::GetTimeFromStart() { return time_.GetTimeFromStart(); }

void ������::SetFullscreen() { /*
  render_manager_->SetFullscreen(true);*/
}

// void ������::RestoreD3DResources() {
//   try {
//     WaitForGpu();
//   } catch (const std::exception &) {
//     // ʲô������
//   }
//   ReleaseD3DResources();
//   OnInit();
// }
//
// void ������::ReleaseD3DResources() {
//   fence_.Reset();
//   ResetComPtrArray(&render_targets_);
//   command_queue_.Reset();
//   swap_chain_.Reset();
//   device_.Reset();
// }
