#include "files.h"
#include <QSqlQuery>

fls::Files::Files(QObject *pobj) : db::DataBase(pobj)
{
  ptrFilesQueryModel_ = new QSqlQueryModel(nullptr);
}

QSqlQueryModel *fls::Files::GetFilesQueryModel()
{
  ConnectToDataBase();
  ptrFilesQueryModel_->setQuery("SELECT * FROM Files;");

  if(ptrFilesQueryModel_->lastError().isValid()){
      qDebug() << ptrFilesQueryModel_->lastError();
      qDebug() << "Model is null.";      
    }

  DisconnectFromDataBase();
  return ptrFilesQueryModel_;
}

bool fls::Files::SaveReceiptToFilesModel(const QVariantList &data)
{
  ConnectToDataBase();
  QSqlQuery query;
   QString str = "INSERT INTO " FILES " ( " FILES_DIAMETER ", "
                                            FILES_THICKNESS ", "
                                            FILES_WIDTH ", "
                                            FILES_NOTE ", "
                                            FILES_DATE ", "
                                            FILES_TIME ", "
                                            FILES_POSITION " ) "
       "VALUES (:Diameter, :Thickness, :Width, :Note, :Date, :Time, :Position )";
   query.prepare(str);

   query.bindValue(":Diameter",   data[0].toDouble());
   query.bindValue(":Thickness",       data[1].toDouble());
   query.bindValue(":Width",           data[2].toDouble());
   query.bindValue(":Note",            data[3].toString());
   query.bindValue(":Date",            data[4].toDate());
   query.bindValue(":Time",            data[5].toTime());
   query.bindValue(":Position",        data[6].toString());


  //ptrFilesQueryModel_->setQuery(query);

  if(!(query.exec())){

      qDebug() << query.lastError().text();
      qDebug() << "Files model didn't save!";

      DisconnectFromDataBase();
      return false;
    }

  qDebug() << "Files model has saved!";

  DisconnectFromDataBase();  

  return true;
}

bool fls::Files::RemoveReceiptFromFilesModel(const int id)
{
  ConnectToDataBase();
  QString strId = QString::number(id);
  ptrFilesQueryModel_->setQuery("UPDATE Files SET IsDeleted = TRUE WHERE id = " + strId + ";");

  if(ptrFilesQueryModel_->lastError().isValid()){
      qDebug() << ptrFilesQueryModel_->lastError();
      qDebug() << "Note doesn't deleted!";
      DisconnectFromDataBase();
      return false;
    }
  qDebug() << "Note deleted!";
  UpdateFilesModel();
  return true;
}

bool fls::Files::RestoreReceiptInFilesModel()
{
  ConnectToDataBase();
  ptrFilesQueryModel_->setQuery("UPDATE Files SET IsDeleted = FALSE;");

  if(ptrFilesQueryModel_->lastError().isValid()){
      qDebug() << ptrFilesQueryModel_->lastError();
      qDebug() << "Notes doesn't restored!";
      DisconnectFromDataBase();
      return false;
    }
  qDebug() << "Notes restored!";
  UpdateFilesModel();
  return true;
}

bool fls::Files::UpdateFilesModel()
{
  ConnectToDataBase();
  ptrFilesQueryModel_->setQuery("SELECT * FROM Files;");

  if(ptrFilesQueryModel_->lastError().isValid()){
      qDebug() << ptrFilesQueryModel_->lastError();
      qDebug() << "Files model didn't update!";
      DisconnectFromDataBase();
      return false;
    }
  qDebug() << "Files model has updated!";
  DisconnectFromDataBase();

  return true;
}
