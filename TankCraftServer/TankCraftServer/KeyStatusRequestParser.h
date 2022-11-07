#pragma once
#include "IParser.h"
class KeyStatusRequestParser : public IParser {
 public:
  KeyStatusRequestParser();
  virtual bool CheckAvailableToParse(const TcpData* pTcpData, int pos) override;

 private:
  virtual IMessage* ParseAtRaw(const TcpData* pTcpData, int pos) override;
};
