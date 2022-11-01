#pragma once

#include <cassert>
#include <memory>
#include <queue>

#include "Component.h"
#include "MyMutex.h"

namespace Xn {

	typedef unsigned int uint;
	typedef unsigned short uint16;
	typedef unsigned char byte;
	typedef wchar_t wchar;

	namespace TanksFight {

		struct NetMessageBaseData {
			uint length;  // 是按wchar的长度
			std::unique_ptr<wchar> data;
		};

		class NetMessageBaseDataBuffer {
		public:
			// 当前数据缓冲是否为空
			inline bool isEmpty() const {
				return datas_.empty();
			}

			// 加入一个数据
			inline void Push(std::unique_ptr<NetMessageBaseData> data) {
				datas_.push(std::move(data));
			}

			// 取出一个数据
			inline std::unique_ptr<NetMessageBaseData> Pop() {
				if (isEmpty()) {
					return nullptr;
				}
				else {
					std::unique_ptr<NetMessageBaseData> data = std::move(datas_.front());
					datas_.pop();
					return data;
				}
			}

		private:
			std::queue<std::unique_ptr<NetMessageBaseData>> datas_;
		};

#define NET_MANAGER_OFFLINE (0) /* mConnectStatus 的可能取值 */
#define NET_MANAGER_ONLINE  (1)

#define NET_MANAGER_BUFFER_CNT (2) /* 两侧缓冲区的个数 */

		class NetManager_Component : public Component {
		public:
			static const uint from_server_datas_buffer_count_;
			static const uint from_client_datas_buffer_count_;

		public:
			NetManager_Component() : 
				Component(L"NetManager_Component"), mConnectStatus(NET_MANAGER_OFFLINE) {}

			// 通过 Component 继承
			virtual void OnStart() override;
			virtual void OnDestory() override;

		public:
			// 连接到服务器
			// 返回:
			//   - 0     : 成功 
			//   - not 0 : 失败
			uint ConnectToServer(const uint16& ipV4_1, const uint16& ipV4_2,
				const uint16& ipV4_3, const uint16& ipV4_4,
				const uint& port);

			// 连接到服务器
			// 参数:
			//   - ipV4 [wstring] : 格式为 [L"1:2:3:4"]，
			//         1、2、3、4分别代表0~255中的一个值的宽字符串格式
			//   - port [wstring] : 格式为 [L"10086"]
			// 返回:
			//   - 0     : 成功
			//   - not 0 : 失败
			uint ConnectToServer(std::wstring ipV4, std::wstring port);

			// 关闭连接
			void DisConnect();

			// 取得服务器到客户端的消息缓冲
			// 约定:
			//   - 取得缓冲后，前一次取得的缓冲指针不再使用
			// 返回:
			//   - nullptr     : 获取失败，可能因为加锁什么的
			//   - not nullptr : 获取成功，指向NetMessageBaseDataBuffer的指针
			NetMessageBaseDataBuffer* const& TryGetServerToClientMessageBuffer();

			// 取得客户端到服务器的消息缓冲
			// 约定:
			//   - 取得缓冲后，前一次取得的缓冲指针不再使用
			// 返回:
			//   - nullptr     : 获取失败，可能因为加锁什么的
			//   - not nullptr : 获取成功，指向NetMessageBaseDataBuffer的指针
			NetMessageBaseDataBuffer* const& TryGetClientToServerMessageBuffer();

			void Lock() const; /* 加锁/解锁 */
			void Unlock() const;

		private:
			int mConnectStatus; /* 子线程根据这一标致退出 */

			uint from_server_datas_buffer_index_ = 0;
			uint from_client_datas_buffer_index_ = 0;
			NetMessageBaseDataBuffer from_server_datas_buffers_[NET_MANAGER_BUFFER_CNT];  // 发送自服务端的缓冲
			NetMessageBaseDataBuffer from_client_datas_buffers_[NET_MANAGER_BUFFER_CNT];  // 发送自客户端的缓冲
			
			MyMutex mMyMutex;//互斥锁
		};

	}  // namespace TanksFight

}  // namespace Xn
