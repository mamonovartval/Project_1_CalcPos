#include "control.h"
#include <unistd.h>

namespace handler {
  Control::Control()
  {
    ptrStm_ = new stm::StandTableModel(nullptr);    
    ptrDb_ = new db::DataBase;
    ptrPs_ = new ps::ProductSize(nullptr);
    ptrFls_ = new fls::Files(nullptr);
    ptrDmt_ = new dmt::DiameterRolls(nullptr);

    updateSavedSvFromDb();

    QString file = "/settings.json";
    process::Reader rd;
    client_.InitSetting(std::get<cmn::SettingOPC>(rd.Read(file)));

    if(!isDebugMode_){
        client_.ClientOn();
      }

    QThreadPool::globalInstance()->start(this);
  }

  void Control::run()
  {
    start();
  }

  stm::StandTableModel* Control::GetStandTableModel()
  {
    return ptrStm_;
  }  

  db::DataBase *Control::GetPtrDataBase()
  {
    return ptrDb_;
  }

  ps::ProductSize *Control::GetPtrProductSize()
  {
    return ptrPs_;
  }

  fls::Files *Control::GetPtrFiles()
  {
    return ptrFls_;
  }

  dmt::DiameterRolls *Control::GetPtrDmt()
  {
    return ptrDmt_;
  }

  Control::~Control()
  {
    delete this;
  }

  void Control::slotSendSv()
  {
    isSendBusy_ = true;
  }

  void Control::slotCalcFromProductSize()
  {
    ptrStm_->UpdateCalcSV(ptrPs_->GetTableSV());

    // for test. when Calculation will be ready, move it to there
    ptrDb_->UpdateDataInDB(SAVED, ptrPs_->GetTableSV());
  }

  void Control::slotLoadData(int id)
  {
    qDebug() << id;
  }

  void Control::start()
  {
     do{
        if(isDebugMode_){
            emit sigDebugMode();
            usleep(microseconds * 250);
            continue;
          }
        // read data from PLC directly
        client_.ReadPV(ptrStm_);
        usleep(microseconds * 100);

        // send data to PLC directly if SendSV = true
        if(isSendBusy_ && !(isDebugMode_)){

            client_.SendSV(ptrStm_);
            isSendBusy_ = false;
          }
      }
    while(client_.GetStatus() == UA_STATUSCODE_GOOD || isDebugMode_); // to think about timer for each model

    if(!isDebugMode_){
        client_.ClientOff();
      }
    qDebug() << "Client Off";
  }

  void Control::updateSavedSvFromDb()
  {
    QHash<QString, double> h_table;

    ptrDb_->UploadDataFromDB(SAVED, h_table);
    ptrStm_->UpdateCalcSV(h_table);
  }
}

