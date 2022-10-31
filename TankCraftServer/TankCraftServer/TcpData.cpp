#include <cassert>
#include <iostream>
#include <string.h>

#include "TcpData.h"

void TcpData::SetData(const char* data, int len)
{
	FreeData(); /* 释放原先的数据 */

	/* len 小于 0 时申请内存会报错 */
	if (len >= 0) {
		mLength = len;
		try {
			mRawData = new char[len];
		}
		catch (...) {
			std::cerr << "TcpData::SetData() MLE, len = " << len << std::endl;
			assert(false);
		}
		memcpy(mRawData, data, len);
	}
}

TcpData::TcpData()
{
	mRawData = nullptr;
	mLength = 0;
}

TcpData::~TcpData()
{
	FreeData();
}

const char* TcpData::GetData() const
{
	return mRawData;
}

int TcpData::GetLength() const
{
	return mLength >= 0 ? mLength : 0;
}

bool TcpData::IsEnd() const
{
	return GetLength() == 0;
}

bool TcpData::GetSanity() const
{
	char oddAns = 0, evenAns = 0;
	for (int i = 0; i < GetLength(); i += 1) {
		/* 下标为偶数 */
		if ((i & 1) == 0) {
			evenAns ^= GetData()[i];
		}
		else {
			oddAns ^= GetData()[i];
		}
	}
	return evenAns == 0 && oddAns == 0; /* 偶校验 */
}

void TcpData::FreeData()
{
	if (mRawData != nullptr) { /* 安全释放 */
		delete[] mRawData;
	}
	mRawData = nullptr;
	mLength = 0;
}
