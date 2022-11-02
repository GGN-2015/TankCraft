#include "stdafx.h"
#include "Ìı¾ıÓï.h"

int WINAPI wWinMain(_In_ HINSTANCE instance_handle, _In_opt_ HINSTANCE,
                    _In_ LPWSTR, _In_ int cmd_show) {
  Xn::Ìı¾ıÓï::Get().Init(instance_handle, 1280, 720);
  return Xn::Ìı¾ıÓï::Get().Run();
}