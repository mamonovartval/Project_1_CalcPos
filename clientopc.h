#ifndef CLIENTOPC_H
#define CLIENTOPC_H
#include "connection.h"
#include "plcdata.h"
#include "common.h"
#include "standtablemodel.h"

/*
There is service which apply manage connection and
manage fetching and sending data with specified PLC by means OPC_UA protocol.
*/

namespace client {
  class ClientOPC: public Connection, public PlcData
  {
  public:
    ClientOPC() = default;

    void InitSetting(const cmn::SettingOPC& settings);
    void ClientOn();
    void ClientOff();
    void ReadPV(stm::StandTableModel* ptrStm);
    void SendSV(const stm::StandTableModel* ptrStm);
  };
}


#endif // CLIENTOPC_H
