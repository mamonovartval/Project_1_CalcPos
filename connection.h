#ifndef CONNECTION_H
#define CONNECTION_H
#include <open62541/client_config_default.h>

#include "common.h"

namespace client {
  class Connection
  {
  protected:
    void SetSetting(const cmn::SettingOPC& setting);
    const UA_Client* GetClient() const;
    UA_Client* GetClient();
    void Connect();
    void Disconnect();

  public:
    Connection();

    const cmn::SettingOPC& GetSetting() const;
    const UA_StatusCode& GetStatus() const;

    ~Connection();

  private:
    UA_Client* ptrClient_;
    cmn::SettingOPC settingOpc_;
    UA_StatusCode statusConnection_ = UA_STATUSCODE_UNCERTAIN;
  };
}


#endif // CONNECTION_H
