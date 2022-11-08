#include "KeyStatusRequest.h"

#include <cassert>
#include <iostream>

#include "GameDatabase.h"
#include "ThreadBuffer.h"

void KeyStatusRequest::GetRawData(TcpData* tcpData) { assert(false); }

int KeyStatusRequest::GetRawDataLength() { return 6; }

void KeyStatusRequest::SetKeyId(unsigned char nKeyId) { mKeyId = nKeyId; }

void KeyStatusRequest::SetKeyStatus(unsigned char nKeyStatus) {
  assert(nKeyStatus == 0 || nKeyStatus == 1);
  mKeyStatus = nKeyStatus;
}

void KeyStatusRequest::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb) {
  std::cerr << "[KeyStatusRequest::Dispatch] Exist" << std::endl;
  if (tb->InGame()) {
    int nUserId = tb->GetUserID();
    Gdb->lock();
    Gdb->SetKeyStatusForUser(nUserId, mKeyId, mKeyStatus);
    Gdb->unlock();
  } else {
    std::cerr << "[KeyStatusRequest::Dispatch] Not InGame" << std::endl;
  }
}

void KeyStatusRequest::DebugShow() {
  std::cerr << "[KeyStatusReqest] "
            << "KeyId = " << (unsigned int)mKeyId << ", "
            << "KeyStatus = " << (unsigned int)mKeyStatus << std::endl;
}
