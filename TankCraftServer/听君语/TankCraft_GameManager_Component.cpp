#include "TankCraft_GameManager_Component.h"

#include "FileAbout.h"
#include "InputManager.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "OutputManager.h"
#include "TankCraft_LoginComponent.h"
#include "TankCraft_MapManager_Component.h"
#include "TankCraft_NetManager_Component.h"
#include "TankCraft_UserManager_Component.h"
#include "Text_RenderComponent.h"
#include "stdafx.h"
#include "������.h"

void Xn::TankCraft::GameManagerComponent::OnStart() {
  game_state_ = GameState::NoConnect;

  user_manager_ = (UserManagerComponent*)GetXnObject()->AddComponent(
      std::make_unique<UserManagerComponent>());
  net_manager_ = (NetManager_Component*)GetXnObject()->AddComponent(
      std::make_unique<NetManager_Component>());

  map_manager_ = (MapManagerComponent*)������::Get()
                     .GetObjectManager()
                     ->CreateXnObject(Vector2::ZERO, GetXnObject())
                     ->AddComponent(std::make_unique<MapManagerComponent>());
  ������::Get()
      .GetObjectManager()
      ->CreateXnObject(Vector2::ZERO, GetXnObject())
      ->AddComponent(std::make_unique<LoginComponent>(this));

  error_message_text_ =
      (Text_RenderComponent*)������::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->SetRenderComponent(std::make_unique<Text_RenderComponent>(
              Vector2(30, 300), L"DebugMessage", Vector2(700, 300), 12.f));
  error_message_text_->SetColor(Vector4(0, 0, 0, 1));

  // TODO ��ʱ�����ö��̲߳�������������bgm���Ȳ�����
  // ulong _ = 0;
  // ReadWavFileIntoMemory(L"�����񘔤ˤʤꤿ��.��Kohana Lam�� - ���Ϥʤ��.wav",
  //                       &bgm_, &_);
  // 
  // ������::Get().GetOutputManager()->PlayAudioWithLoop(bgm_);
}
void Xn::TankCraft::GameManagerComponent::OnUpdate() {
  if (const auto buffer = net_manager_->TryGetServerToClientMessageBuffer()) {
    NetMessageDeal(buffer);
  }

  if (game_state_ == GameState::Gaming) TrySendKeyMessage();
}
void Xn::TankCraft::GameManagerComponent::OnDestory() {
  GetXnObject()->RemoveAllChild();

  // delete[] bgm_;
}

void Xn::TankCraft::GameManagerComponent::ConnectToServer(
    const std::wstring ipV4, const std::wstring port) {
  if (game_state_ != GameState::NoConnect) {
    error_message_text_->SetText(L"�������ӻ����ڳ�������");
    return;
  }

  if (ipV4.size() <= 0) {
    error_message_text_->SetText(L"IP��ȡʧ��");
    return;
  }
  if (port.size() <= 0) {
    error_message_text_->SetText(L"port��ȡʧ��");
    return;
  }

  net_manager_->ConnectToServer(ipV4, port);
  game_state_ = GameState::WaitForConnect;
}
void Xn::TankCraft::GameManagerComponent::Login(const std::wstring user_name) {
  if (game_state_ != GameState::Connected) {
    error_message_text_->SetText(L"δ���ӻ����ڵ�¼���ѵ�¼");
    return;
  }

  if (user_name.size() <= 0) {
    error_message_text_->SetText(L"��һ�µ�¼��");
    return;
  }

  auto buffer = net_manager_->TryGetClientToServerMessageBuffer();
  if (!buffer) return;
  // ��¼����
  {
    auto data = std::make_unique<NetMessageBaseData>();
    const uint message_type = 1;
    const uint name_length_as_char = 2 * (uint)user_name.size();

    std::wstring s = L"";
    s.push_back(*(wchar*)&message_type);
    s.push_back(*(wchar*)&name_length_as_char);
    s += user_name;
    data->SetData(s.data(), (uint)s.size());
    buffer->Push(std::move(data));
    game_state_ = GameState::WaitForLogin;
  }
  // ��ͼ����
  {
    auto data = std::make_unique<NetMessageBaseData>();
    const uint message_type = 2;
    const uint name_length_as_char = 0;

    std::wstring s = L"";
    s.push_back(*(wchar*)&message_type);
    s.push_back(*(wchar*)&name_length_as_char);
    data->SetData(s.data(), (uint)s.size());
    buffer->Push(std::move(data));
  }
  // ���������Ϣ
  ������::Get().GetInputManager()->keyboard_manager_.Flush();
}

void Xn::TankCraft::GameManagerComponent::NetMessageDeal(
    NetMessageBaseDataBuffer* const& server_to_client) {
  for (auto message = server_to_client->Pop(); message.get();
       message = server_to_client->Pop()) {
    const wchar* data = message->data;
    uint index = 0;

    const uint msg_type = *(uint16*)&data[0];
    switch (msg_type) {
      case 65535: {
        const uint error_code = *(uint16*)&data[2];
        InternalCommunicationMessage(error_code);
      } break;

      case 0: {
        const uint return_x = *(uint16*)&data[2];
        // TODO Ping����
      } break;

      case 1: {
        const uint login_error_code = *(uint16*)&data[2];
        const uint user_id = *(uint*)&data[3];
        DealLoginMessage(login_error_code, user_id);
      } break;

      case 2: {
        const uint wh = *(uint16*)&data[2];
        const uint width = (wh >> 8) & 0xFF;
        const uint height = wh & 0xFF;
        SetMap(&data[3], width, height);
      } break;

      case 3: {
        const uint user_count = *(uint16*)&data[2];
        AddUsers(&data[3], user_count);
      } break;

      case 4: {
        const uint tank_count = *(uint16*)&data[2];
        SetTanksState(&data[3], tank_count);
      } break;

      case 5: {
        const uint this_user_kill_number = *(uint16*)&data[2];
        const uint user_kill_number_count = *(uint16*)&data[3];
        SetUsersKillNumber(this_user_kill_number, &data[4],
                           user_kill_number_count);
      } break;

      case 6: {
        // ����������Ϣ������
      } break;

      case 7: {
        const uint entitiy_count = *(uint16*)&data[2];
        SetEntitiesState(&data[3], entitiy_count);
      } break;

      default:
        break;
    }
  }
}

void Xn::TankCraft::GameManagerComponent::InternalCommunicationMessage(
    const uint& code) {
  OutputDebugString(std::to_wstring(code).data());
  switch (code) {
    case 0: {
      game_state_ = GameState::Connected;
      // TODO ���������ӳɹ�
      {
        error_message_text_->SetText(L"���ӳɹ�");
        return;
      }
    } break;
    case 1: {
      game_state_ = GameState::NoConnect;
      // TODO WAS����ʱ����
      {
        error_message_text_->SetText(L"WAS����ʱ����");
        return;
      }
    } break;
    case 2: {
      game_state_ = GameState::NoConnect;
      // TODO �׽��ֶ�����ʱ����
      {
        error_message_text_->SetText(L"�׽��ֶ�����ʱ����");
        return;
      }
    } break;
    case 3: {
      game_state_ = GameState::NoConnect;
      // TODO ���Ӳ���������
      {
        error_message_text_->SetText(L"���Ӳ���������");
        return;
      }
    } break;
    case 4: {
      game_state_ = GameState::NoConnect;
      // TODO �����ӵ������������ǶϿ�������
      {
        error_message_text_->SetText(L"�����ӵ������������ǶϿ�������");
        return;
      }
    } break;

    default: {
      // δ֪��Ϣ
    } break;
  }
}
void Xn::TankCraft::GameManagerComponent::SetMap(const wchar* const& map_data,
                                                 const uint& x_side_length,
                                                 const uint& y_side_length) {
  map_manager_->SetMap(map_data, x_side_length, y_side_length);
}
void Xn::TankCraft::GameManagerComponent::AddUsers(
    const wchar* const& users_data, const uint& user_count) {
  uint users_data_index = 0;

  for (uint add_user_index = 0; add_user_index < user_count; ++add_user_index) {
    const auto user_id = *(uint*)&users_data[users_data_index];
    users_data_index += 2;

    const auto user_name_length = *(uint16*)&users_data[users_data_index];
    users_data_index += 1;

    const auto user_name = &users_data[users_data_index];
    users_data_index += user_name_length;

    union RGBA {
      struct {
        uint8 r;
        uint8 g;
        uint8 b;
        uint8 a;
      };
      char data[4];
    } the_color = *(RGBA*)&users_data[users_data_index];
    const Vector4 user_color(the_color.r / 255.f, the_color.g / 255.f,
                             the_color.b / 255.f, the_color.a / 255.f);
    users_data_index += 2;

    const auto user_kill_number = *(uint16*)&users_data[users_data_index];
    users_data_index += 1;

    AddUser(user_id, user_name_length, user_name, user_color, user_kill_number);
  }
}
void Xn::TankCraft::GameManagerComponent::SetTanksState(
    const wchar* const& tanks_data, const uint& tank_count) {
  uint tank_data_index = 0;

  map_manager_->StartSyncTankState();
  for (uint tank_index = 0; tank_index < tank_count; ++tank_index) {
    const auto user_id = *(uint*)&tanks_data[tank_data_index];
    tank_data_index += 2;

    union Pos {
      struct {
        Float x;
        Float y;
      };
      char data[8];
    } the_pos = *(Pos*)&tanks_data[tank_data_index];
    const Vector2 tank_pos(the_pos.x, the_pos.y);
    tank_data_index += 4;

    const auto tank_rotation =
        180.f * *(Float*)&tanks_data[tank_data_index] / Float::PI;
    tank_data_index += 2;

    const auto tank_state = *(uint16*)&tanks_data[tank_data_index];
    tank_data_index += 1;

    map_manager_->SetTankState(user_id, tank_pos, tank_rotation, tank_state);
  }
  map_manager_->EndSyncTankState();
}
void Xn::TankCraft::GameManagerComponent::SetUsersKillNumber(
    const uint& this_user_kill_number,
    const wchar* const& users_kill_number_data,
    const uint& users_kill_number_count) {
  uint users_kill_number_data_index = 0;

  for (uint users_index = 0; users_index < users_kill_number_count;
       ++users_index) {
    const auto user_id =
        *(uint*)&users_kill_number_data[users_kill_number_data_index];
    users_kill_number_data_index += 2;

    const auto users_kill_number =
        *(uint16*)&users_kill_number_data[users_kill_number_data_index];
    users_kill_number_data_index += 1;

    user_manager_->GetUser(user_id)->kill_number = users_kill_number;
  }
}
void Xn::TankCraft::GameManagerComponent::SetEntitiesState(
    const wchar* const& entities_data, const uint& entity_count) {
  map_manager_->StartSyncBulletState();

  uint entities_data_index = 0;
  for (uint i = 0; i < entity_count; ++i) {
    const uint entity_type = *(uint16*)&entities_data[entities_data_index];
    entities_data_index += 1;
    switch (entity_type) {
      case 0: {  // �ӵ�
        const Float x = *(Float*)&entities_data[entities_data_index];
        entities_data_index += 2;
        const Float y = *(Float*)&entities_data[entities_data_index];
        entities_data_index += 2;
        const uint id = *(uint16*)&entities_data[entities_data_index];
        entities_data_index += 1;
        SetBulletState(id, {x, y});
      } break;
      case 1: {  // �к�
        const Float x = *(Float*)&entities_data[entities_data_index];
        entities_data_index += 2;
        const Float y = *(Float*)&entities_data[entities_data_index];
        entities_data_index += 2;
        const uint id = *(uint*)&entities_data[entities_data_index];
        entities_data_index += 2;
        SetTankBomb(id, {x, y});
      } break;
      case 2: {
        const uint laser_point_count =
            *(uint16*)&entities_data[entities_data_index];
        entities_data_index += 1;
        SetLaser(&entities_data[entities_data_index], laser_point_count);
        entities_data_index += 4 * laser_point_count;
      } break;
      case 3: {
        // TODO����
        assert("����������");
      } break;

      default:
        break;
    }
  }

  map_manager_->EndSyncBulletState();
}

void Xn::TankCraft::GameManagerComponent::DealLoginMessage(
    const uint& login_error_code, const uint& user_id) {
  switch (login_error_code) {
    case 0: {
      game_state_ = GameState::Gaming;
      // ��¼�ɹ�
      // TODO
      {
        error_message_text_->SetText(L"��¼�ɹ�");
        return;
      }
    } break;

    case 1: {
      game_state_ = GameState::Connected;
      // ��������Ա
      // TODO
      {
        error_message_text_->SetText(L"��������Ա");
        return;
      }
    } break;

    case 2: {
      game_state_ = GameState::Connected;
      // ��¼���ظ�
      // TODO
      {
        error_message_text_->SetText(L"��¼���ظ�");
        return;
      }
    } break;

    case 3: {
      game_state_ = GameState::Connected;
      // ���������ø�IP
      // TODO
      {
        error_message_text_->SetText(L"���������ø�IP");
        return;
      }
    } break;

    case 4: {
      game_state_ = GameState::Connected;
      // ����֮��Ĵ���
      // TODO
      {
        error_message_text_->SetText(L"��½ʱ����������������֮��Ĵ���");
        return;
      }
    } break;

    default:
      break;
  }
}

void Xn::TankCraft::GameManagerComponent::TrySendKeyMessage() {
  if (!������::Get().GetInputManager()->keyboard_manager_.IsKeyBufferEmpty()) {
    auto buffer = net_manager_->TryGetClientToServerMessageBuffer();
    if (!buffer) return;

    while (!������::Get()
                .GetInputManager()
                ->keyboard_manager_.IsKeyBufferEmpty()) {
      auto key = ������::Get().GetInputManager()->keyboard_manager_.ReadKey();

      auto data = std::make_unique<NetMessageBaseData>();
      const uint message_type = 6;
      const uint length = 2;

      uint virtual_key = -1;
      uint key_state = -1;

      switch (key.GetCode()) {
        case VK_SPACE: {
          virtual_key = 0;
        } break;
        case VK_W: {
          virtual_key = 2;
        } break;
        case VK_S: {
          virtual_key = 4;
        } break;
        case VK_A: {
          virtual_key = 3;
        } break;
        case VK_D: {
          virtual_key = 1;
        } break;
      }
      if (virtual_key == -1) continue;

      if (key.IsPress())
        key_state = 0;
      else if (key.IsRelease())
        key_state = 1;
      if (key_state == -1) continue;

      const uint the_key = virtual_key | (key_state << 8);

      {
        std::wstring mes = L"���ͣ�������Ϣ��";
        mes += L"������";
        mes += std::to_wstring(virtual_key);
        mes += L"��";
        mes += L"״̬��";
        mes += std::to_wstring(key_state);
        mes += L"��";
        error_message_text_->SetText(mes);
        OutputDebugString((mes + L"\n").data());
      }

      std::wstring s = L"";
      s.push_back(*(wchar*)&message_type);
      s.push_back(*(wchar*)&length);
      s.push_back(*(wchar*)&the_key);

      data->SetData(s.data(), (int)s.size());
      buffer->Push(std::move(data));
    }
  }
}

void Xn::TankCraft::GameManagerComponent::AddUser(
    const uint user_id, const uint& user_name_length,
    const wchar* const& user_name, const Vector4& user_color,
    const uint& user_kill_number) {
  std::wstring name(user_name_length, '\0');
  memcpy(name.data(), user_name, size_t(2 * user_name_length));

  const auto user_data =
      user_manager_->AddUser(user_id, name, user_color, user_kill_number);

  map_manager_->TryBindUser(user_data);
}
void Xn::TankCraft::GameManagerComponent::SetBulletState(const uint& id,
                                                         const Vector2& pos) {
  map_manager_->SetBulletState(id, pos);
}
void Xn::TankCraft::GameManagerComponent::SetTankBomb(const uint& user_id,
                                                      const Vector2& pos) {
  // TODO
}
void Xn::TankCraft::GameManagerComponent::SetLaser(
    const wchar* const& laser_data, const uint& point_count) {
  // TODO
}
