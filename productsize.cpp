#include "productsize.h"
#include <QSqlRecord>

namespace ps {

  ProductSize::ProductSize(QObject* pobj) : db::DataBase(pobj)
  {
    ptrThicknessQueryModel_ = new QSqlQueryModel(nullptr);
    ptrDiamTubeQueryModel_ = new QSqlQueryModel(nullptr);

    ptrWidthStripsQueryModel_ = new QSqlQueryModel(nullptr);
    ptrDiamTubeAtSqQueryModel_ = new QSqlQueryModel(nullptr);
  }

  QString ProductSize::GetDiamTube()
  {
    return ptrDiamTubeQueryModel_->record(Column::First).value("Diam_value").toString();
  }

  QString ProductSize::GetDiamAtSQ(const QString &currentPipeDiameter)
  {
    const QString nameConnection = "GetDiamAtSQ";
    ConnectToDataBase();

    QString nameTable = FFX;
    QString str_query = "SELECT Diameter_at_SQ "
                        "FROM " + nameTable +
        " WHERE Diameter_tube = " + currentPipeDiameter;
    ptrDiamTubeAtSqQueryModel_->setQuery(str_query);

    // verify query to DB
    if(ptrDiamTubeAtSqQueryModel_->lastError().isValid()){
        qDebug() << ptrDiamTubeAtSqQueryModel_->lastError();
      }

    DisconnectFromDataBase();

    return ptrDiamTubeAtSqQueryModel_->record(Column::First).value("Diameter_at_SQ").toString();
  }

  QString ProductSize::GetWidth(const QString &currentPipeDiameter, const QString &currentWallThickness)
  {
    const QString nameConnection = "Getwidth";
    ConnectToDataBase();

    QString nameTable = FFX;
    QString str_query = "SELECT Width "
                        "FROM " + nameTable +
                        " WHERE Diameter_tube = " + currentPipeDiameter
                        + " AND Thickness = " + currentWallThickness;

    ptrWidthStripsQueryModel_->setQuery(str_query);

    //verify query to DB
    if(ptrWidthStripsQueryModel_->lastError().isValid()){
        qDebug() << ptrWidthStripsQueryModel_->lastError();
      }

    DisconnectFromDataBase();

    return ptrWidthStripsQueryModel_->record(Column::First).value("Width").toString();
  }

  QSqlQueryModel *ProductSize::GetModelDiamTube()
  {
    const QString nameConnection = "GetModelDiamTube";
    ConnectToDataBase();

    ptrDiamTubeQueryModel_->setQuery("SELECT Diam_value FROM DiameterTube;");

    // verify query to DB
    if(ptrDiamTubeQueryModel_->lastError().isValid()){
        qDebug() << ptrDiamTubeQueryModel_->lastError();
      }

    DisconnectFromDataBase();

    return ptrDiamTubeQueryModel_;
  }

  QSqlQueryModel *ProductSize::GetModelThickness()
  {
    const QString nameConnection = "GetModelThickness";
    ConnectToDataBase();

    ptrThicknessQueryModel_->setQuery("SELECT Thickness FROM WallThickness;");

    // verify query to DB
    if(ptrThicknessQueryModel_->lastError().isValid()){
        qDebug() << ptrThicknessQueryModel_->lastError();        
      }

    DisconnectFromDataBase();

    return ptrThicknessQueryModel_;
  }

  void ProductSize::SetCurrentProduct(const QString &currentPipeDiameter, const QString &currentWallThickness)
  {
    currentProduct_.diameter = currentPipeDiameter.toDouble();
    currentProduct_.wallThickness = currentWallThickness.toDouble();
  }

  const QHash<QString, double>& ProductSize::GetTableSV()
  {
    getSVFromTablesDB(currentProduct_);
    return tableSV_;
  }

  void ProductSize::getSVFromTablesDB(const cmn::Product &currentProduct)
  {
    setDataToTableSvFromDb(FFX, currentProduct);
    setDataToTableSvFromDb(FPSQ, currentProduct);
  }

  void ProductSize::setDataToTableSvFromDb(const QString &nameTable, const cmn::Product &currentProduct)
  {
    const QString nameConnection = "setDataToTableSvFromDb";
    ConnectToDataBase();

    QSqlQuery query;
    QString str_diam = QString::number(currentProduct.diameter);
    QString str = "SELECT * "
                  "FROM " + nameTable +
        " WHERE Diameter_tube = " + str_diam;

    // verify wall thickness for select in FFX table
    if(nameTable == FFX && currentProduct.wallThickness != 0){
        QString str_thickness = QString::number(currentProduct.wallThickness);
        // update string query
        str = "SELECT * "
              "FROM " + nameTable +
            " WHERE Diameter_tube = " + str_diam +
            " AND Thickness = " + str_thickness;
      }

    if(!query.exec(str)){
        qDebug() << "Unable to execute query - exiting";
      }

    QSqlRecord rec = query.record();

    // reading from end DB
    while (query.next()){
        // verify condition
        assert(currentProduct.diameter == query.value(rec.indexOf("Diameter_tube")).toDouble());
        //write data
        tableSV_[query.value(rec.indexOf("Name_axis")).toString()] =
                query.value(rec.indexOf("Original_SV")).toDouble();
      }

    DisconnectFromDataBase();
  }
}

