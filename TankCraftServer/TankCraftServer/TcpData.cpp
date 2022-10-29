#include <string.h>

#include "TcpData.h"

void TcpData::SetData(const char* data, int len)
{
	FreeData(); /* �ͷ�ԭ�ȵ����� */

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
	if (mRawData != nullptr) { /* ��ȫ�ͷ� */
		delete[] mRawData;
	}
	mRawData = nullptr;
	mLength = 0;
}
