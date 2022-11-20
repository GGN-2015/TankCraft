#include "OutputManager-AudioManager.h"

#include "FileAbout.h"
#include "ForDebug.h"

Xn::AudioManager::AudioManager(const HWND& window_handle) {
  InitDSound(window_handle);
}

IDirectSoundBuffer* Xn::AudioManager::LoadWaveAudio(
    const wchar* const& wave_path) {
  IDirectSoundBuffer* d_sound_buffer = nullptr;

  byte* wave_file_data;
  ulong wave_file_data_size;
  // 读文件
  {
    std::ifstream f(wave_path, std::ios::binary);

    f.seekg(0, std::ios::end);
    wave_file_data_size = f.tellg();
    f.seekg(0, std::ios::beg);

    wave_file_data = new byte[wave_file_data_size];
    f.read((char*)wave_file_data, wave_file_data_size);

    f.close();
  }

  WAVEFORMATEX* wave_format = nullptr;
  byte* wave_data = nullptr;
  ulong wave_data_size = 0;
  // 解析wave文件
  {
    ulong *memory_pointer, *memory_end;
    ulong riff, type, length;

    memory_pointer = (ulong*)wave_file_data;
    riff = *memory_pointer++;
    length = *memory_pointer++;
    type = *memory_pointer++;

    ulong tmp1 = mmioFOURCC('R', 'I', 'F', 'F');
    ulong tmp2 = mmioFOURCC('W', 'A', 'V', 'E');

    if (riff != mmioFOURCC('R', 'I', 'F', 'F')) THROW_XN_ERROR();
    if (type != mmioFOURCC('W', 'A', 'V', 'E')) THROW_XN_ERROR();

    memory_end = (ulong*)((byte*)wave_file_data + length - 4);

    bool is_bend = false;
    while ((memory_pointer < memory_end) && (!is_bend)) {
      type = *memory_pointer++;
      length = *memory_pointer++;
      switch (type) {
        case mmioFOURCC('f', 'm', 't', ' '): {
          if (wave_format) break;

          if (length < sizeof(WAVEFORMAT)) THROW_XN_ERROR();
          wave_format = (WAVEFORMATEX*)memory_pointer;
        } break;

        case mmioFOURCC('d', 'a', 't', 'a'): {
          if (wave_data && wave_data_size) break;

          wave_data = (byte*)memory_pointer;
          wave_data_size = length;
          if (wave_format) is_bend = true;
        } break;
      }
      memory_pointer = (ulong*)((byte*)memory_pointer + ((length + 1) & ~1));
    }
  }

  DSBUFFERDESC buffer_desc{.dwSize = sizeof(DSBUFFERDESC),
                           .dwFlags = 0,
                           .dwBufferBytes = wave_data_size,
                           .lpwfxFormat = wave_format};
  XN_THROW_IF_FAILED(
      d_sound_->CreateSoundBuffer(&buffer_desc, &d_sound_buffer, 0));

  void* audio1;
  ulong len1;
  void* audio2;
  ulong len2;
  XN_THROW_IF_FAILED(d_sound_buffer->Lock(0, wave_data_size, &audio1, &len1,
                                          &audio2, &len2, 0));

  std::memcpy(audio1, wave_data, len1);
  if (len2 > 0) {
    byte* data2 = (byte*)wave_data + len1;
    std::memcpy(audio2, data2, len2);
  }
  d_sound_buffer->Unlock(audio1, len1, audio2, len2);

  delete[] wave_file_data;

  return d_sound_buffer;
}

void Xn::AudioManager::InitDSound(const HWND& window_handle) {
  XN_THROW_IF_FAILED(DirectSoundCreate(NULL, &d_sound_, NULL));

  XN_THROW_IF_FAILED(d_sound_->SetCooperativeLevel(
      window_handle, DSSCL_PRIORITY));  // DSSCL_NORMAL
}
