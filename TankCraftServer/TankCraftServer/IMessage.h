#pragma once
class IMessage
{
public:
	virtual int  GetRawDataLength() = 0; /* 根据数据长度定位下一个包 */
	virtual void Dispatch() = 0;         /* 算法逻辑核心：根据数据修改程序存储的信息 */
};
