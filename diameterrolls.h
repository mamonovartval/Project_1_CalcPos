#ifndef DIAMETERROLLS_H
#define DIAMETERROLLS_H

#include <QObject>
#include <database.h>
#include <QSqlQueryModel>

namespace dmt {

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

  class DiameterRolls : db::DataBase
  {
    Q_OBJECT
  public:
    DiameterRolls(QObject* pobj = 0);

    QSqlQueryModel* GetFfxDiamQueryModel();
    QSqlQueryModel* GetFpSqDiamQueryModel(const QString& currDiam);

    bool EntryDiamEnable();

  private:
    QSqlQueryModel* ptrFfxDiamQueryModel_;
    QSqlQueryModel* ptrFpSqDiamQueryModel_;

    bool uploadDiamInModel();
  };
}


#endif // DIAMETERROLLS_H
