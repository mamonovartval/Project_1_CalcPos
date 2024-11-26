#ifndef FILES_H
#define FILES_H

#include <QObject>
#include "database.h"
#include <QSqlQueryModel>

namespace fls {

  enum Column{
    First,
    Second,
    Third,
    Fourth,
    Fifth,
    Sixth,
    Seventh,
    Eighth,
    Ninth
  };

  class Files : public db::DataBase
  {
    Q_OBJECT
  public:
    Files(QObject* pobj = 0);

    QSqlQueryModel* GetFilesQueryModel();

    bool SaveReceiptToFilesModel(const QVariantList& data);
    bool RemoveReceiptFromFilesModel(const int id);
    bool LoadReceiptFromFilesModel();
    bool RestoreReceiptInFilesModel();
    bool UpdateFilesModel();
    bool SortReceiptInFilesModel(const QString& arg);
    bool UpShiftReceiptInFilesModel(const QSqlRecord& rec);
    bool DownShiftReceiptInFilesModel(const QSqlRecord& rec);

  private:
    QSqlQueryModel* ptrFilesQueryModel_;
  };
}


#endif // FILES_H
