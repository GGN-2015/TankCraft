#pragma once
#include "MyMutex.h"

/* GameDatabase ��������Ϸ������ĵĵ��� */
class GameDatabase
{
public:
	static GameDatabase* GetGlobalGameDatabase(); /* ��ȡ���� */

private:
	GameDatabase();

	static GameDatabase* pGlobalGameDatabase; /* ָ�뵥�� */
	MyMutex mMyMutex;                         /* ���е�д��������Ҫ���� */
};
