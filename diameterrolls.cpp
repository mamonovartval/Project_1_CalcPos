#include "diameterrolls.h"

dmt::DiameterRolls::DiameterRolls(QObject *pobj) : db::DataBase(pobj)
{
  ptrFfxDiamQueryModel_ = new QSqlQueryModel(this);
  ptrFpSqDiamQueryModel_ = new QSqlQueryModel(this);
}

QSqlQueryModel *dmt::DiameterRolls::GetFfxDiamQueryModel()
{
  ConnectToDataBase();
  ptrFfxDiamQueryModel_->setQuery("SELECT * FROM FFXDIAM;");

  if(ptrFfxDiamQueryModel_->lastError().isValid()){
      qDebug() << ptrFfxDiamQueryModel_->lastError();
    }

  DisconnectFromDataBase();
  return ptrFfxDiamQueryModel_;
}

QSqlQueryModel *dmt::DiameterRolls::GetFpSqDiamQueryModel(const QString& currDiam)
{
  ConnectToDataBase();
  ptrFpSqDiamQueryModel_->setQuery("SELECT * FROM FPSQ WHERE Diameter_tube = " + currDiam + ";");

  if(ptrFpSqDiamQueryModel_->lastError().isValid()){
      qDebug() << ptrFpSqDiamQueryModel_->lastError();
    }

  DisconnectFromDataBase();
  return ptrFpSqDiamQueryModel_;
}

bool dmt::DiameterRolls::EntryDiamEnable()
{
  uploadDiamInModel();
  return false;
}

bool dmt::DiameterRolls::uploadDiamInModel()
{
  return false; // to do
}
