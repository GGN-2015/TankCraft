#include <windows.h>
#include "MyMutex.h"

MyMutex::MyMutex()
{
	/* ´´½¨ÄäÃû mutex */
	pMutex = (void*)CreateMutex(nullptr, false, nullptr);
}

MyMutex::~MyMutex()
{
	CloseHandle(pMutex);
}

void MyMutex::lock() const
{
	DWORD d = WaitForSingleObject(pMutex, INFINITE);
}

void MyMutex::unlock() const
{
	ReleaseMutex(pMutex);
}
