#pragma once
class MyMutex
{
public:
	MyMutex();
	~MyMutex();

	void lock() const;   /* ���� */
	void unlock() const; /* ���� */

private:
	/* ���� HANDLE �� windows.h ��ʵ����������������ֻ��ʹ�� void* */
	void* pMutex; /* һ��ָ�� mutex �ľ�� */
};
