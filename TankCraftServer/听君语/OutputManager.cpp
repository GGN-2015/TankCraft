#include "OutputManager.h"

Xn::OutputManager::OutputManager() {}

void Xn::OutputManager::PlayAudio(const byte* const& audio_bytes) {
  audio_manager_.PlayAudio(audio_bytes);
}

void Xn::OutputManager::PlayAudioWithLoop(const byte* const& audio_bytes) {
  audio_manager_.PlayMusic(audio_bytes);
}

void Xn::OutputManager::StopAllAudio() { audio_manager_.StopAll(); }
