#ifndef PRODUCTSIZE_H
#define PRODUCTSIZE_H

#include <QObject>
#include "database.h"
#include <QSqlQueryModel>
#include <QSqlTableModel>

namespace ps {

  enum Column{
    First,
    Second,
    Third,
    Fourth,
    Fifth,
    Sixth,
    Seventh,
    Eighth ,
    Ninth,
    Tenth
  };

  class ProductSize : public db::DataBase
  {
    Q_OBJECT
  public:
    ProductSize(QObject* pobj = 0);

    QString GetDiamTube();
    QStringList GetThicknesses();
    QString GetDiamAtSQ(const QString& currentPipeDiameter);
    QString GetWidth(const QString& currentPipeDiameter, const QString& currentWallThickness);
    QSqlQueryModel *GetModelDiamTube();
    QSqlQueryModel *GetModelThickness();
    QSqlTableModel* GetFfxTableModel();
    QSqlTableModel* GetFpsqTableModel();

    void SetCurrentProduct(const QString& currentPipeDiameter,
                           const QString& currentWallThickness);

    const QHash<QString, double>& GetTableSV();

  private:
    QSqlQueryModel* ptrThicknessQueryModel_;
    QSqlQueryModel* ptrDiamTubeQueryModel_;

    QSqlQueryModel* ptrWidthStripsQueryModel_;
    QSqlQueryModel* ptrDiamTubeAtSqQueryModel_;

    QSqlTableModel* ptrFfxTableModel_;
    QSqlTableModel* ptrFpsqTableModel_;

    QHash<QString, double> tableSV_;
    cmn::Product currentProduct_;

    void getSVFromTablesDB(const cmn::Product& nameColumn);
    void setDataToTableSvFromDb(const QString &nameTable, const cmn::Product &nameColumn);
  };
}


#endif // PRODUCTSIZE_H
