#include "TanksFight_NetManager_Component.h"
using namespace Xn;
using namespace Xn::TanksFight;

const uint NetManager_Component::from_server_datas_buffer_count_ = NET_MANAGER_BUFFER_CNT;
const uint NetManager_Component::from_client_datas_buffer_count_ = NET_MANAGER_BUFFER_CNT;

void Xn::TanksFight::NetManager_Component::DisConnect()
{
	mConnectStatus = NET_MANAGER_OFFLINE; /* 关闭客户端的连接 */
}

void Xn::TanksFight::NetManager_Component::Lock() const
{
	mMyMutex.lock();
}

void Xn::TanksFight::NetManager_Component::Unlock() const
{
	mMyMutex.unlock();
}
