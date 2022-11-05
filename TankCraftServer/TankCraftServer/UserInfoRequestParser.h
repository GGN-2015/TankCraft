#pragma once
#include "IParser.h"
class UserInfoRequestParser : public IParser {
 public:
  UserInfoRequestParser();
  virtual bool CheckAvailableToParse(const TcpData* pTcpData, int pos) override;

 private:
  virtual IMessage* ParseAtRaw(const TcpData* pTcpData, int pos) override;
};
