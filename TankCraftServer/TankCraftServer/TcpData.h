#pragma once
class TcpData
{
public:
	TcpData();
	~TcpData();
	
	void SetData(const char* data, int len); /* ���� ���� */
	const char* GetData() const; /* ��ȡԭʼ���� */
	int GetLength() const;       /* ��ȡ���ݳ��� */
	bool IsEnd() const;          /* ����Ƿ��ǽ�β�� */
	bool GetSanity() const;      /* ���У��� */
	void FreeData();

private:
	int   mLength;
	char* mRawData;
};
