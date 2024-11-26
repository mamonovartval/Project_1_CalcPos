#include "connection.h"

namespace client {
  Connection::Connection()
  {
    UA_Client* newClient = UA_Client_new();
    ptrClient_ = std::move(newClient);
    UA_ClientConfig_setDefault(UA_Client_getConfig(ptrClient_));
  }

  void Connection::Connect()
  {
    std::string adress = "opc.tcp://" + settingOpc_.ip_adress.toStdString() + ":" + settingOpc_.port_adress.toStdString();
    statusConnection_ = UA_Client_connect(ptrClient_, adress.c_str());
  }

  void Connection::Disconnect()
  {
    statusConnection_ = UA_Client_disconnect(ptrClient_);
  }

  void Connection::SetSetting(const cmn::SettingOPC& setup)
  {
    settingOpc_.ip_adress = setup.ip_adress;
    settingOpc_.port_adress = setup.port_adress;
  }

  const cmn::SettingOPC &Connection::GetSetting() const
  {
    return settingOpc_;
  }

  const UA_StatusCode &Connection::GetStatus() const
  {
    return statusConnection_;
  }

  const UA_Client *Connection::GetClient() const
  {
    return ptrClient_;
  }

  UA_Client *Connection::GetClient()
  {
    return ptrClient_;
  }

  Connection::~Connection()
  {
    UA_Client_delete(ptrClient_);
  }
}

