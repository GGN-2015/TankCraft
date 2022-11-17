#include "OutputManager-AudioManager.h"

#include "stdafx.h"

Xn::AudioManager::AudioManager() {}

void Xn::AudioManager::PlayAudio(const byte* const& audio_bytes) {
  PlaySound((LPCWSTR)audio_bytes, NULL, SND_MEMORY | SND_ASYNC);
}
void Xn::AudioManager::PlayMusic(const byte* const& audio_bytes) {
  auto b =
      PlaySound((LPCWSTR)audio_bytes, NULL, SND_MEMORY | SND_ASYNC | SND_LOOP);
  OutputDebugString((L"≤•∑≈“Ù¿÷£∫" + std::to_wstring(b) + L"!!!!!!!!!\n").data());
}

void Xn::AudioManager::StopAll() { PlaySoundW(NULL, NULL, 0); }
