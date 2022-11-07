#include "SysUtils.h"

#include <windows.h>

#include <cstdlib>
#include <ctime>

void SysUtils::Sleep(int milliSeconds) { ::Sleep(milliSeconds); }

void SysUtils::SRand() { srand((unsigned int)time(NULL)); }
