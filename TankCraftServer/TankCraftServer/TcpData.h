#pragma once
class TcpData
{
public:
	TcpData();
	~TcpData();
	
	void SetData(const char* data, int len); /* 设置 数据 */
	const char* GetData() const; /* 获取原始数据 */
	int GetLength() const;       /* 获取数据长度 */
	bool IsEnd() const;          /* 检查是否是结尾符 */
	bool GetSanity() const;      /* 检查校验和 */
	void FreeData();

	void DebugShow(const char* prompt) const;         /* 用于对于数据内容的显示 */

	unsigned short GetUnsignedShortAt(int pos) const; /* 获取一个无符号短整数 */

private:
	int   mLength;
	char* mRawData;
};
