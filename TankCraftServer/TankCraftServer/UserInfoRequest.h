#pragma once
#include "IMessage.h"
class UserInfoRequest :
    public IMessage
{
public:
	virtual int  GetRawDataLength();                            /* 需要根据数据长度定位下一个包 */
	virtual void Dispatch(ThreadBuffer* tb, GameDatabase* Gdb); /* 算法逻辑核心：根据数据修改程序存储的信息 */

	virtual void GetRawData(TcpData* tcpData); /* 获取二进制数据 */
	virtual void DebugShow() override;         /* 调试专用输出 */
};
