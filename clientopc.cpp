#include "clientopc.h"

namespace client {
  void ClientOPC::InitSetting(const cmn::SettingOPC &settings)
  {
    SetSetting(std::move(settings));
  }

  void ClientOPC::ClientOn()
  {
    Connect();
  }

  void ClientOPC::ClientOff()
  {
    Disconnect();
  }


  void ClientOPC::ReadPV(stm::StandTableModel *ptrStm)
  {
    for(int i = 0; i < ptrStm->rowCount(QModelIndex()); ++i){
        QModelIndex index_1 = ptrStm->index(i, stm::First);
        // create full name format tag for read from PLC
        QString nameTag = index_1.data().toString();
        // create format tag for connection
        cmn::Variable fullNameTag(ptrStm->GetNameDB(), nameTag);
        // read and update data inside container
        QModelIndex index_3 = ptrStm->index(i, stm::Third);
        QVariant value(ReadData(GetClient(), detail::node, fullNameTag.GetVariable()));
        value = value.toDouble() / 1000.0;
        ptrStm->setData(index_3, value, Qt::EditRole);
    }
    qDebug() << "Data is reading.";
  }

  void ClientOPC::SendSV(const stm::StandTableModel *ptrStm)
  {
    for(int i = 0; i < ptrStm->rowCount(QModelIndex()); ++i){
        QModelIndex index_1 = ptrStm->index(i, stm::First);
        // create full name format tag for read from PLC
        QString nameTag = index_1.data().toString();
        // create format tag for connection
        cmn::Variable fullNameTag(ptrStm->GetNameDB(), nameTag);
        // get index for third column from container
        QModelIndex index_4 = ptrStm->index(i, stm::Fourth);
        // read data from container and convert from double to int
        UA_Int32 value = index_4.data().toDouble() * 1000;
        WriteData(GetClient(), detail::node, fullNameTag.GetVariable(), value);
    }
    qDebug() << "Data has written.";
  }

}

