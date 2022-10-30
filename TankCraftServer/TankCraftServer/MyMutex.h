#pragma once
class MyMutex
{
public:
	MyMutex();
	~MyMutex();

	void lock() const;   /* 加锁 */
	void unlock() const; /* 解锁 */

private:
	/* 由于 HANDLE 在 windows.h 中实现所以我们在这里只能使用 void* */
	void* pMutex; /* 一个指向 mutex 的句柄 */
};
