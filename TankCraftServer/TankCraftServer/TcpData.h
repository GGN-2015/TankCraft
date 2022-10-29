#pragma once
class TcpData
{
public:
	TcpData();
	~TcpData();
	
	void SetData(const char* data, int len); /* 设置 数据 */
	const char* GetData() const; /* 获取原始数据 */
	int GetLength() const; /* 获取数据长度 */
	bool isEnd() const; /* 检查是否是结尾符 */
	void FreeData();

private:
	int   mLength;
	char* mRawData;
};
