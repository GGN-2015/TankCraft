#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "ForDebug.h"

namespace Xn {

#define SAFE_RELEASE(p) \
  if (p) (p)->Release()

// 取得资源路径
inline void GetAssetsPath(wchar *path, uint path_size) {
  if (!path) throw std::exception();

  ulong size = GetModuleFileName(nullptr, path, path_size);
  if (size == 0 || size == path_size) {
    // 函数失败或路径被截断
    throw std::exception();
  }

  wchar *last_slash_index = wcsrchr(path, L'\\');
  if (last_slash_index) *(last_slash_index + 1) = L'\0';
}

// 从文件中读取数据
inline HRESULT ReadDataFromFile(const wchar *filename, byte **data,
                                uint *size) {
  using namespace Microsoft::WRL;

#if WINVER >= _WIN32_WINNT_WIN8
  CREATEFILE2_EXTENDED_PARAMETERS extended_params = {};
  extended_params.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
  extended_params.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
  extended_params.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
  extended_params.dwSecurityQosFlags = SECURITY_ANONYMOUS;
  extended_params.lpSecurityAttributes = nullptr;
  extended_params.hTemplateFile = nullptr;

  Wrappers::FileHandle file(CreateFile2(filename, GENERIC_READ, FILE_SHARE_READ,
                                        OPEN_EXISTING, &extended_params));
#else
  Wrappers::FileHandle file(CreateFile(
      file_name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN |
          SECURITY_SQOS_PRESENT | SECURITY_ANONYMOUS,
      nullptr));
#endif  // WINVER >= _WIN32_WINNT_WIN8
  if (file.Get() == INVALID_HANDLE_VALUE) throw std::exception();

  FILE_STANDARD_INFO file_info = {};
  if (!GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &file_info,
                                    sizeof(file_info)))
    throw std::exception();

  if (file_info.EndOfFile.HighPart != 0) throw std::exception();

  *data = reinterpret_cast<byte *>(malloc(file_info.EndOfFile.LowPart));
  *size = file_info.EndOfFile.LowPart;

  if (!ReadFile(file.Get(), *data, file_info.EndOfFile.LowPart, nullptr,
                nullptr))
    throw std::exception();

  return S_OK;
}

// 从DDS文件中读取数据
inline HRESULT ReadDataFromDDSFile(const wchar *filename, byte **data,
                                   uint *offset, uint *size) {
  if (FAILED(ReadDataFromFile(filename, data, size))) return E_FAIL;

  // DDS文件的开头数字是固定的：0x20534444
  static constexpr uint dds_magic = 0x20534444;
  uint magic_number = *reinterpret_cast<const uint *>(*data);
  if (magic_number != dds_magic) return E_FAIL;

  struct DdsPixelFormat {
    uint size;
    uint flags;
    uint fourCC;
    uint rgb_bit_count;
    uint r_bit_mask;
    uint g_bit_mask;
    uint b_bit_mask;
    uint a_bit_mask;
  };

  struct DdsHeader {
    uint size;
    uint flags;
    uint height;
    uint width;
    uint pitch_or_linear_size;
    uint depth;
    uint mip_map_count;
    uint reserved1[11];
    DdsPixelFormat dds_pixel_format;
    uint caps;
    uint caps2;
    uint caps3;
    uint caps4;
    uint reserved2;
  };

  auto dds_header = reinterpret_cast<const DdsHeader *>(*data + sizeof(uint));
  if (dds_header->size != sizeof(DdsHeader) ||
      dds_header->dds_pixel_format.size != sizeof(DdsPixelFormat))
    return E_FAIL;

  constexpr long dds_data_offset = sizeof(uint) + sizeof(DdsHeader);
  *offset = dds_data_offset;
  *size = *size - dds_data_offset;

  return S_OK;
}

// 将字节大小对齐为8的倍数
// 样例：
//     uint aligment_byte_size =
//     CalculateConstantBufferByteSize(no_aligment_byte_size);
inline uint CalculateConstantBufferByteSize(uint byte_size) {
  return (byte_size + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) &
         ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
}

#ifdef D3D_COMPILE_STANDARD_FILE_INCLUDE
inline ComPtr<ID3DBlob> CompileShader(const std::wstring &filename,
                                      const D3D_SHADER_MACRO *defines,
                                      const std::string &entrypoint,
                                      const std::string &target) {
  uint compile_flags = 0;
#ifdef _DEBUG
  compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif  // _DEBUG

  HRESULT hr;

  ComPtr<ID3DBlob> byte_code = nullptr;
  ComPtr<ID3DBlob> errors;
  hr =
      D3DCompileFromFile(filename.c_str(), defines,
                         D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint.c_str(),
                         target.c_str(), compile_flags, 0, &byte_code, &errors);

  if (errors != nullptr) OutputDebugStringA((char *)errors->GetBufferPointer());
  XN_THROW_IF_FAILED(hr);

  return byte_code;
}
#endif  // D3D_COMPILE_STANDARD_FILE_INCLUDE

// 对ComPtr数组中的所有元素调用Reset()
template <class T>
void ResetComPtrArray(T *com_ptr_array) {
  for (auto &item : *com_ptr_array) item.Reset();
}

// 对unique_ptr数组中的所有元素调用Reset()
template <class T>
void ResetUniquePtrArray(T *unique_ptr_array) {
  for (auto &item : *unique_ptr_array) item.reset();
}

}  // namespace Xn
