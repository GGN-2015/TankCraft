#include <string.h>

#include "TcpData.h"

void TcpData::SetData(const char* data, int len)
{
	FreeData(); /* 释放原先的数据 */

	mLength = len;
	mRawData = new char[len];
	memcpy(mRawData, data, len);
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
	return mLength;
}

bool TcpData::isEnd() const
{
	return GetLength() == 0;
}

void TcpData::FreeData()
{
	if (mRawData != nullptr) { /* 安全释放 */
		delete[] mRawData;
	}
	mRawData = nullptr;
	mLength = 0;
}
