#include "PlatformManager.h"
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#ifdef _WIN64

#include "InputManager.h"
#include "resource.h"
#include "������.h"

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
    //-----���ڴ���-----
    case WM_CREATE: {
      // ����PlatformWindowָ��
      LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(param2);
      SetWindowLongPtr(
          window_handle, GWLP_USERDATA,
          reinterpret_cast<LONG_PTR>(create_struct->lpCreateParams));

      SetTimer(window_handle, 1, 8, NULL);
    } break;

    //-----��������-----
    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    //-----�����ƶ�-----
    case WM_MOVE: {
      const int x = static_cast<int>(LOWORD(param2));
      const int y = static_cast<int>(HIWORD(param2));
      PlatformWindowCppGetInterface::SetPos(platform_window, x, y);

      ������::Get().OnWindowMoved(x, y);
    } break;

    //-----��������-----
    case WM_SIZE: {
      RECT client_rect = {};
      GetClientRect(window_handle, &client_rect);
      const int width = client_rect.right - client_rect.left;
      const int height = client_rect.bottom - client_rect.top;
      PlatformWindowCppGetInterface::SetWH(platform_window, width, height);

      ������::Get().OnSizeChanged(width, height, false);
    } break;

    //-----����ʧȥ����-----
    case WM_KILLFOCUS:
      ������::Get().GetInputManager()->ClearState();
      break;

    //-----���ڽ��հ���������Ϣ-----
    case WM_KEYDOWN:
      if (!(param2 & 0x40000000))
        ������::Get().GetInputManager()->OnKeyDown(static_cast<byte>(param1));
      break;

    //-----���ڽ��հ���̧����Ϣ-----
    case WM_KEYUP:
      ������::Get().GetInputManager()->OnKeyUp(static_cast<byte>(param1));
      break;

    //-----���ڽ����ַ���Ϣ-----
    case WM_CHAR:
      ������::Get().GetInputManager()->OnChar(static_cast<wchar>(param1));
      break;

    //-----���ڽ��հ���������Ϣ-----
    case WM_SYSKEYDOWN:
      if (!(param2 & 0x40000000))
        ������::Get().GetInputManager()->OnKeyDown(static_cast<byte>(param1));
      break;

      //-----��ʱ��ˢ�´���-----
    case WM_TIMER:
      ������::Get().Update();
      ������::Get().Render();
      break;

    //-----���ڽ�������ƶ���Ϣ-----
    case WM_MOUSEMOVE: {
      const POINTS pt = MAKEPOINTS(param2);
      if (pt.x >= 0 && pt.x < platform_window->GetWidth() && pt.y >= 0 &&
          pt.y < platform_window->GetHeight()) {
        ������::Get().GetInputManager()->OnMouseMove(pt.x, pt.y);
        if (!������::Get().GetInputManager()->IsMouseInWindow()) {
          SetCapture(window_handle);
          ������::Get().GetInputManager()->OnMouseEnter();
        }
      } else {
        if (param1 & (MK_LBUTTON | MK_RBUTTON))
          ������::Get().GetInputManager()->OnMouseMove(pt.x, pt.y);
        else {
          ReleaseCapture();
          ������::Get().GetInputManager()->OnMouseLeave();
        }
      }
    } break;

    //-----���ڽ���������������Ϣ-----
    case WM_LBUTTONDOWN: {
      const POINTS pt = MAKEPOINTS(param2);

      ������::Get().GetInputManager()->OnMouseLeftPressed(pt.x, pt.y);
    } break;

    //-----���ڽ���������̧����Ϣ-----
    case WM_LBUTTONUP: {
      const POINTS pt = MAKEPOINTS(param2);

      ������::Get().GetInputManager()->OnMouseLeftReleased(pt.x, pt.y);
    } break;

    //-----���ڽ�������Ҽ�������Ϣ-----
    case WM_RBUTTONDOWN: {
      const POINTS pt = MAKEPOINTS(param2);

      ������::Get().GetInputManager()->OnMouseRightPressed(pt.x, pt.y);
    } break;

    //-----���ڽ�������Ҽ�̧����Ϣ-----
    case WM_RBUTTONUP: {
      const POINTS pt = MAKEPOINTS(param2);

      ������::Get().GetInputManager()->OnMouseRightReleased(pt.x, pt.y);
    } break;

      //-----���ڽ�����������Ϣ-----
    case WM_MOUSEWHEEL: {
      const POINTS pt = MAKEPOINTS(param2);
      const int delta = GET_WHEEL_DELTA_WPARAM(param1);
      ������::Get().GetInputManager()->OnMouseWheel(pt.x, pt.y, delta);
    } break;

    // ����δ���������Ϣ
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
  // ��ʼ��������
  WNDCLASSEX window_class = {};
  window_class.cbSize = sizeof(WNDCLASSEX);  // ������ô�ֵ
  window_class.style = 0 /* CS_NOCLOSE*/;  // ���ô��ڲ˵��ϵĹرռ�
  window_class.lpfnWndProc = WindowProcess;  // ���ڹ���
  window_class.hInstance = instance_handle;  // ʵ�����
  window_class.hIcon = static_cast<HICON>(LoadImage(
      instance_handle, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
  window_class.hCursor = LoadCursor(NULL, IDC_ARROW);     // �����ʽ
  window_class.lpszClassName = ������::Get().core_name_;  // ��������
  window_class.hIconSm = static_cast<HICON>(LoadImage(
      instance_handle, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
  RegisterClassEx(&window_class);

  RECT window_rect = {0, 0, width, height};
  AdjustWindowRect(&window_rect, window_style_, FALSE);

  width_ = window_rect.right - window_rect.left;
  height_ = window_rect.bottom - window_rect.top;
  // �������ڲ�ȡ�ô��ھ��
  window_handle_ = CreateWindowEx(
      WS_EX_NOREDIRECTIONBITMAP, ������::Get().core_name_,
      window_title_.c_str(), window_style_, CW_USEDEFAULT, CW_USEDEFAULT,
      width_, height_, nullptr, nullptr, instance_handle, this);

  GetWindowRect(window_handle_, &window_rect);
  x_ = window_rect.left;
  y_ = window_rect.top;
}

int PlatformManager::Run() {
  ShowWindow(window_handle_, SW_NORMAL);

  // ����Ϣѭ��
  MSG message = {};
  while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) &&
         message.message != WM_QUIT) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
  while (message.message != WM_QUIT) {
    // ��������е���Ϣ
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) &&
           message.message != WM_QUIT) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

  ������::Get().OnDestroy();

  // ����WM_QUIT�е���Ϣ
  return static_cast<char>(message.wParam);
}

std::wstring PlatformManager::GetWindowTitle() { return window_title_; }
Vector2Int PlatformManager::GetPos() { return Vector2Int(x_, y_); }
int Xn::PlatformManager::GetWidth() { return width_; }
int Xn::PlatformManager::GetHeight() { return height_; }
const bool PlatformManager::IsFullscreen() { return is_fullscreen_; }

const HWND PlatformManager::GetWindowHandle() { return window_handle_; }

void PlatformManager::SetFullscreen(bool is_fullscreen) {
  // û��ת��ȫ��״̬�����Բ���
  if (is_fullscreen == is_fullscreen_) return;

  if (is_fullscreen) {  // ȫ��
    GetWindowRect(window_handle_, &window_rect_before_fullscreen_);

    // ����������Ϊ�ޱ߿�
    SetWindowLong(
        window_handle_, GWL_STYLE,
        window_style_ & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX |
                          WS_SYSMENU | WS_THICKFRAME));

    RECT fullscreen_window_rect;
    // ȡ������ʾ��������
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
  } else {  // ȡ��ȫ��
    // �ָ���������
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
                "������::PlatformWindow::SetWindowZorder(const WindowZorder "
                "&window_zorder) : ����֮���Z��");

  SetWindowPos(window_handle_, windows_window_zorder_flag, x_, y_, width_,
               height_, SWP_FRAMECHANGED | SWP_NOACTIVATE);
}

#endif  // _WIN64
