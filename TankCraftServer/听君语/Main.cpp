#include "stdafx.h"
#include "������.h"

int WINAPI wWinMain(_In_ HINSTANCE instance_handle, _In_opt_ HINSTANCE,
                    _In_ LPWSTR, _In_ int cmd_show) {
  Xn::������::Get().Init(instance_handle, 1280, 720);
  return Xn::������::Get().Run();
}