#include "PlatformManager.h"
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#ifdef _WIN64

#include "InputManager.h"
#include "resource.h"
#include "听君语.h"

using namespace Xn;

class PlatformWindowCppGetInterface {
 public:
  static void SetPos(PlatformManager *that, int x, int y) {
    that->x_ = x;
    that->y_ = y;
  }
  static void SetX(PlatformManager *that, int x) { that->x_ = x; }
  static void SetY(PlatformManager *that, int y) { that->y_ = y; }
  static void SetWH(PlatformManager *that, int width, int height) {
    that->width_ = width;
    that->height_ = height;
  }
  static void SetWidth(PlatformManager *that, int width) {
    that->width_ = width;
  }
  static void SetHeight(PlatformManager *that, int height) {
    that->height_ = height;
  }
};

int64 CALLBACK WindowProcess(HWND window_handle, uint message, WPARAM param1,
                             LPARAM param2) {
  PlatformManager *platform_window = reinterpret_cast<PlatformManager *>(
      GetWindowLongPtr(window_handle, GWLP_USERDATA));

  switch (message) {
    //-----窗口创建-----
    case WM_CREATE: {
      // 保存PlatformWindow指针
      LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(param2);
      SetWindowLongPtr(
          window_handle, GWLP_USERDATA,
          reinterpret_cast<LONG_PTR>(create_struct->lpCreateParams));

      SetTimer(window_handle, 1, 8, NULL);
    } break;

    //-----窗口销毁-----
    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    //-----窗口移动-----
    case WM_MOVE: {
      const int x = static_cast<int>(LOWORD(param2));
      const int y = static_cast<int>(HIWORD(param2));
      PlatformWindowCppGetInterface::SetPos(platform_window, x, y);

      听君语::Get().OnWindowMoved(x, y);
    } break;

    //-----窗口缩放-----
    case WM_SIZE: {
      RECT client_rect = {};
      GetClientRect(window_handle, &client_rect);
      const int width = client_rect.right - client_rect.left;
      const int height = client_rect.bottom - client_rect.top;
      PlatformWindowCppGetInterface::SetWH(platform_window, width, height);

      听君语::Get().OnSizeChanged(width, height, false);
    } break;

    //-----窗口失去焦点-----
    case WM_KILLFOCUS:
      听君语::Get().GetInputManager()->ClearState();
      break;

    //-----窗口接收按键按下消息-----
    case WM_KEYDOWN:
      if (!(param2 & 0x40000000))
        听君语::Get().GetInputManager()->OnKeyDown(static_cast<byte>(param1));
      break;

    //-----窗口接收按键抬起消息-----
    case WM_KEYUP:
      听君语::Get().GetInputManager()->OnKeyUp(static_cast<byte>(param1));
      break;

    //-----窗口接收字符消息-----
    case WM_CHAR:
      听君语::Get().GetInputManager()->OnChar(static_cast<wchar>(param1));
      break;

    //-----窗口接收按键按下消息-----
    case WM_SYSKEYDOWN:
      if (!(param2 & 0x40000000))
        听君语::Get().GetInputManager()->OnKeyDown(static_cast<byte>(param1));
      break;

      //-----定时器刷新窗口-----
    case WM_TIMER:
      听君语::Get().Update();
      听君语::Get().Render();
      break;

    //-----窗口接收鼠标移动消息-----
    case WM_MOUSEMOVE: {
      const POINTS pt = MAKEPOINTS(param2);
      if (pt.x >= 0 && pt.x < platform_window->GetWidth() && pt.y >= 0 &&
          pt.y < platform_window->GetHeight()) {
        听君语::Get().GetInputManager()->OnMouseMove(pt.x, pt.y);
        if (!听君语::Get().GetInputManager()->IsMouseInWindow()) {
          SetCapture(window_handle);
          听君语::Get().GetInputManager()->OnMouseEnter();
        }
      } else {
        if (param1 & (MK_LBUTTON | MK_RBUTTON))
          听君语::Get().GetInputManager()->OnMouseMove(pt.x, pt.y);
        else {
          ReleaseCapture();
          听君语::Get().GetInputManager()->OnMouseLeave();
        }
      }
    } break;

    //-----窗口接收鼠标左键按下消息-----
    case WM_LBUTTONDOWN: {
      const POINTS pt = MAKEPOINTS(param2);

      听君语::Get().GetInputManager()->OnMouseLeftPressed(pt.x, pt.y);
    } break;

    //-----窗口接收鼠标左键抬起消息-----
    case WM_LBUTTONUP: {
      const POINTS pt = MAKEPOINTS(param2);

      听君语::Get().GetInputManager()->OnMouseLeftReleased(pt.x, pt.y);
    } break;

    //-----窗口接收鼠标右键按下消息-----
    case WM_RBUTTONDOWN: {
      const POINTS pt = MAKEPOINTS(param2);

      听君语::Get().GetInputManager()->OnMouseRightPressed(pt.x, pt.y);
    } break;

    //-----窗口接收鼠标右键抬起消息-----
    case WM_RBUTTONUP: {
      const POINTS pt = MAKEPOINTS(param2);

      听君语::Get().GetInputManager()->OnMouseRightReleased(pt.x, pt.y);
    } break;

      //-----窗口接收鼠标滚轮消息-----
    case WM_MOUSEWHEEL: {
      const POINTS pt = MAKEPOINTS(param2);
      const int delta = GET_WHEEL_DELTA_WPARAM(param1);
      听君语::Get().GetInputManager()->OnMouseWheel(pt.x, pt.y, delta);
    } break;

    // 处理未被捕获的信息
    default:
      return DefWindowProc(window_handle, message, param1, param2);
  }
  return 0;
}

Xn::PlatformManager::PlatformManager(const HINSTANCE &instance_handle,
                                     std::wstring window_title,
                                     const int &width, const int &height)
    : window_title_(window_title),
      window_style_(WS_OVERLAPPEDWINDOW),
      is_fullscreen_(false),
      window_rect_before_fullscreen_({}) {
  // 初始化窗口类
  WNDCLASSEX window_class = {};
  window_class.cbSize = sizeof(WNDCLASSEX);  // 务必设置此值
  window_class.style = 0 /* CS_NOCLOSE*/;  // 禁用窗口菜单上的关闭键
  window_class.lpfnWndProc = WindowProcess;  // 窗口过程
  window_class.hInstance = instance_handle;  // 实例句柄
  window_class.hIcon = static_cast<HICON>(LoadImage(
      instance_handle, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
  window_class.hCursor = LoadCursor(NULL, IDC_ARROW);     // 鼠标样式
  window_class.lpszClassName = 听君语::Get().core_name_;  // 窗口类名
  window_class.hIconSm = static_cast<HICON>(LoadImage(
      instance_handle, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
  RegisterClassEx(&window_class);

  RECT window_rect = {0, 0, width, height};
  AdjustWindowRect(&window_rect, window_style_, FALSE);

  width_ = window_rect.right - window_rect.left;
  height_ = window_rect.bottom - window_rect.top;
  // 创建窗口并取得窗口句柄
  window_handle_ = CreateWindowEx(
      WS_EX_NOREDIRECTIONBITMAP, 听君语::Get().core_name_,
      window_title_.c_str(), window_style_, CW_USEDEFAULT, CW_USEDEFAULT,
      width_, height_, nullptr, nullptr, instance_handle, this);

  GetWindowRect(window_handle_, &window_rect);
  x_ = window_rect.left;
  y_ = window_rect.top;
}

int PlatformManager::Run() {
  ShowWindow(window_handle_, SW_NORMAL);

  // 主消息循环
  MSG message = {};
  while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) &&
         message.message != WM_QUIT) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
  while (message.message != WM_QUIT) {
    // 处理队列中的消息
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) &&
           message.message != WM_QUIT) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

  听君语::Get().OnDestroy();

  // 返回WM_QUIT中的信息
  return static_cast<char>(message.wParam);
}

std::wstring PlatformManager::GetWindowTitle() { return window_title_; }
Vector2Int PlatformManager::GetPos() { return Vector2Int(x_, y_); }
int Xn::PlatformManager::GetWidth() { return width_; }
int Xn::PlatformManager::GetHeight() { return height_; }
const bool PlatformManager::IsFullscreen() { return is_fullscreen_; }

const HWND PlatformManager::GetWindowHandle() { return window_handle_; }

void PlatformManager::SetFullscreen(bool is_fullscreen) {
  // 没有转变全屏状态，忽略操作
  if (is_fullscreen == is_fullscreen_) return;

  if (is_fullscreen) {  // 全屏
    GetWindowRect(window_handle_, &window_rect_before_fullscreen_);

    // 将窗口设置为无边框
    SetWindowLong(
        window_handle_, GWL_STYLE,
        window_style_ & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX |
                          WS_SYSMENU | WS_THICKFRAME));

    RECT fullscreen_window_rect;
    // 取得主显示器的设置
    {
      DEVMODE dev_mode = {};
      dev_mode.dmSize = sizeof(DEVMODE);
      EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dev_mode);

      fullscreen_window_rect = {
          dev_mode.dmPosition.x, dev_mode.dmPosition.y,
          dev_mode.dmPosition.x + static_cast<long>(dev_mode.dmPelsWidth),
          dev_mode.dmPosition.y + static_cast<long>(dev_mode.dmPanningHeight)};
    }

    SetWindowPos(window_handle_, HWND_TOPMOST, fullscreen_window_rect.left,
                 fullscreen_window_rect.top, fullscreen_window_rect.right,
                 fullscreen_window_rect.bottom,
                 SWP_FRAMECHANGED | SWP_NOACTIVATE);

    ShowWindow(window_handle_, SW_MAXIMIZE);
  } else {  // 取消全屏
    // 恢复窗口属性
    SetWindowLong(window_handle_, GWL_STYLE, window_style_);

    const long window_width = window_rect_before_fullscreen_.right -
                              window_rect_before_fullscreen_.left;
    const long window_height = window_rect_before_fullscreen_.bottom -
                               window_rect_before_fullscreen_.top;
    SetWindowPos(window_handle_, HWND_NOTOPMOST,
                 window_rect_before_fullscreen_.left,
                 window_rect_before_fullscreen_.top, window_width,
                 window_height, SWP_FRAMECHANGED | SWP_NOACTIVATE);

    ShowWindow(window_handle_, SW_NORMAL);
  }

  is_fullscreen_ = is_fullscreen;
}
void PlatformManager::SetWindowZorder(const WindowZorder &window_zorder) {
  HWND windows_window_zorder_flag =
      window_zorder == WindowZorder::Bottom      ? HWND_BOTTOM
      : window_zorder == WindowZorder::Top       ? HWND_TOP
      : window_zorder == WindowZorder::NoTopmost ? HWND_NOTOPMOST
      : window_zorder == WindowZorder::Topmost
          ? HWND_TOPMOST
          : throw new std::exception(
                "听君语::PlatformWindow::SetWindowZorder(const WindowZorder "
                "&window_zorder) : 意料之外的Z序！");

  SetWindowPos(window_handle_, windows_window_zorder_flag, x_, y_, width_,
               height_, SWP_FRAMECHANGED | SWP_NOACTIVATE);
}

#endif  // _WIN64
