#include "database.h"
#include <QCoreApplication>
#include <QSqlRecord>

namespace db {
  DataBase::DataBase(QObject *pobj) : QObject(pobj){}

  QVariant DataBase::ConnectToDataBase()
  {
    /* Before connecting to a database checks for its existence.
     * Depending on the result of the opening of manufacture database or its recovery
     * */
    QString path = DATABASE_NAME;
    QFile file(path);

    if(!file.exists()){
        if(this->restoreDataBase()){

            return QVariant(RESTORE);
          }
        else{
            return QVariant();
          }
      }
    else{
        if(this->openDataBase()){

            return QVariant(OPEN);
          }
        else{
            return QVariant();
          }
      }
  }

  void DataBase::DisconnectFromDataBase()
  {
    closeDataBase();
  }

  bool DataBase::InsertIntoFFXTable(const QVariantList &data)
  {
    /* SQL Query formed from QVariantList,
         * which are transmitted in data to be inserted into the table.
         * */
    QSqlQuery query;
        /* e SQL query is generated beginning with keys,
         * which then bind with bindValue method for substituting data from QVariantList               TABLE_NO ", "
         * */
     QString str = "INSERT INTO " FFX " ( " FFX_DIAMETER ", "
                                              FFX_DIAMSQ ", "
                                              FFX_THICKNESS ", "
                                              FFX_WIDTH ", "
                                              FFX_TAGNAME ", "
                                              FFX_ORSV " ) "
         "VALUES (:Diameter_tube, :Diameter_at_SQ, :Thickness, :Width, :Name_axis, :Original_SV )";
     query.prepare(str);

     query.bindValue(":Diameter_tube",   data[0].toDouble());
     query.bindValue(":Diameter_at_SQ",  data[1].toDouble());
     query.bindValue(":Thickness",       data[2].toDouble());
     query.bindValue(":Width",           data[3].toDouble());
     query.bindValue(":Name_axis",       data[4].toString());
     query.bindValue(":Original_SV",     data[5].toDouble());

    if(!query.exec()){
        qDebug() << "Error insert into " << FFX;
        qDebug() << query.lastError().text();
        return false;
      }
    return true;
  }

  bool DataBase::InsertIntoFPSQTable(const QVariantList &data)
  {
    /* SQL Query formed from QVariantList,
         * which are transmitted in data to be inserted into the table.
         * */
    QSqlQuery query;
        /* e SQL query is generated beginning with keys,
         * which then bind with bindValue method for substituting data from QVariantList               TABLE_NO ", "
         * */
     QString str = "INSERT INTO " FPSQ " ( " FPSQ_DIAMETER ", "
                                              FPSQ_TAGNAME ", "
                                              FPSQ_ORDIAM ", "
                                              FPSQ_PRDIAM ", "
                                              FPSQ_ORSV ", "
                                              FPSQ_ENDIAM " ) "
         "VALUES (:Diameter_tube, :Name_axis, :Original_diameter, :Present_diameter, :Original_SV, :Entry_diameter )";
     query.prepare(str);

     query.bindValue(":Diameter_tube",     data[0].toDouble());
     query.bindValue(":Name_axis",         data[1].toString());
     query.bindValue(":Original_diameter", data[2].toDouble());
     query.bindValue(":Present_diameter",  data[3].toDouble());
     query.bindValue(":Original_SV",       data[4].toDouble());
     query.bindValue(":Entry_diameter",    data[5].toDouble());

    if(!query.exec()){
        qDebug() << "Error insert into " << FPSQ;
        qDebug() << query.lastError().text();
        return false;
      }
    return true;
  }

  bool DataBase::InsertIntoFFXDiamTable(const QVariantList &data)
  {
    /* SQL Query formed from QVariantList,
         * which are transmitted in data to be inserted into the table.
         * */
    QSqlQuery query;
        /* e SQL query is generated beginning with keys,
         * which then bind with bindValue method for substituting data from QVariantList               TABLE_NO ", "
         * */
     QString str = "INSERT INTO " FFXDIAM " ( " FFXDIAM_TYPDATA ", "
                                              FFXDIAM_TAGNAME ", "
                                              FFXDIAM_ORDIAM ", "
                                              FFXDIAM_PRDIAM ", "
                                              FFXDIAM_ENDIAM " ) "
         "VALUES (:Type_data, :Name_axis, :Original_diameter, :Present_diameter, :Entry_diameter )";
     query.prepare(str);

     query.bindValue(":Type_data",         data[0].toString());
     query.bindValue(":Name_axis",         data[1].toString());
     query.bindValue(":Original_diameter", data[2].toDouble());
     query.bindValue(":Present_diameter",  data[3].toDouble());
     query.bindValue(":Entry_diameter",    data[4].toDouble());

    if(!query.exec()){
        qDebug() << "Error insert into " << FFXDIAM;
        qDebug() << query.lastError().text();
        return false;
      }
    return true;
  }

  bool DataBase::InsertIntoSavedTable(const QVariantList &data)
  {
    /* SQL Query formed from QVariantList,
         * which are transmitted in data to be inserted into the table.
         * */
    QSqlQuery query;
        /* e SQL query is generated beginning with keys,
         * which then bind with bindValue method for substituting data from QVariantList               TABLE_NO ", "
         * */
     QString str = "INSERT INTO " SAVED " ( " SAVED_TAGNAME ", "
                                              SAVED_SV " ) "
         "VALUES (:Name_axis, :Saved_SV)";
     query.prepare(str);

     query.bindValue(":Name_axis",         data[0].toString());
     query.bindValue(":Saved_SV",          data[1].toString());

    if(!query.exec()){
        qDebug() << "Error insert into " << SAVED;
        qDebug() << query.lastError().text();
        return false;
      }
    return true;
  }

  const QHash<QString, double> DataBase::GetDataFromDB(const QString &nameTable,
                                                       const cmn::Product &nameColumn)
  {
    QHash<QString, double> result;
    QSqlQuery query;
    QString str_diam = QString::number(nameColumn.diameter);
    QString str = "SELECT * "
                  "FROM " + nameTable +
        " WHERE Diameter_tube = " + str_diam;

    // verify wall thickness for select in FFX table
    if(nameColumn.wallThickness != 0){
        QString str_thickness = QString::number(nameColumn.wallThickness);
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
        assert(nameColumn.diameter == query.value(rec.indexOf("Diameter_tube")).toDouble());
        //write data
        result[query.value(rec.indexOf("Name_axis")).toString()] =
                query.value(rec.indexOf("Original_SV")).toDouble();
      }

    return result;
  }

  bool DataBase::UpdateDataInDB(const QString &nameTable, const QHash<QString, double> &data)
  {
    ConnectToDataBase();
    QSqlQuery query;

    int counter = 1;
    for(const auto& key : data.keys()){
        QString str_sv = QString::number(data.value(key));
        QString str = "UPDATE " + nameTable + " SET Saved_SV = :Saved_SV, Name_axis = :Name_axis WHERE id = :id ";

        query.prepare(str);

        query.bindValue(":Name_axis", key);
        query.bindValue(":Saved_SV", str_sv);
        query.bindValue(":id", counter);

        if(!(query.exec())){
            qDebug() << "Unable to execute query - exiting";
            qDebug() << query.lastError().text();
            return false;
          }

        ++counter;
      }

    DisconnectFromDataBase();
    return true;
  }

  bool DataBase::UploadDataFromDB(const QString &nameTable, QHash<QString, double> &data)
  {
    ConnectToDataBase();
    QSqlQuery query;
    QString str = "SELECT * "
                  "FROM " + nameTable;

    if(!query.exec(str)){
        qDebug() << "Unable to execute query - exiting";
        qDebug() << query.lastError().text();
        return false;
      }

    QSqlRecord rec = query.record();

    // reading from end DB
    while (query.next()){

        //write data
        data[query.value(rec.indexOf("Name_axis")).toString()] =
                query.value(rec.indexOf("Saved_SV")).toDouble();
      }

    DisconnectFromDataBase();
    return true;
  }

  const QSqlDatabase &DataBase::GetDB()
  {
    return db_;
  }

  bool DataBase::openDataBase()
  {
    /* Database opens along a predetermined path and the database name, if it exists
     * */

    qDebug() << "Before open DB: " << db_.connectionName();

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setHostName(DATABASE_HOSTNAME);
    db_.setDatabaseName(DATABASE_NAME);

    if(!db_.open()){
        qDebug() << "Cannot open database: " << db_.lastError().text();
        return false;
      }

    qDebug() << "After open DB: " << db_.connectionName();

    return true;
  }

  bool DataBase::restoreDataBase()
  {
    if(this->openDataBase() && this->createTables())
      {
        return true;
      }

    qDebug() << "Failed to restore the database";
    return false;
  }

  void DataBase::closeDataBase()
  {
    qDebug() << "Before close: " << db_.connectionName()
             << " " << db_.connectionNames().size();


   // db_.close();
    db_.removeDatabase(db_.connectionName());


    qDebug() << "After close: " << db_.connectionName()
             << " " << db_.connectionNames().size();
  }

  bool DataBase::createTables()
  {
    return createTableFiles() && createTableFFX() && createTableFPSQ()
       && createTableFFXDiam() && createTableSavedSV();
  }

  bool DataBase::createTableFiles()
  {
    /* In this case, a forming raw SQL-query with its subsequent execution.
         * */                  //TABLE_NO " INTEGER NOT NULL,"
    QSqlQuery query;

    // table Files
    QString str = "CREATE TABLE " FILES " ("
                                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                  FILES_DIAMETER " DOUBLE NOT NULL,"
                                                  FILES_THICKNESS " DOUBLE NOT NULL,"
                                                  FILES_WIDTH " DOUBLE NOT NULL,"
                                                  FILES_NOTE " VARCHAR(255) NOT NULL,"
                                                  FILES_DATE " DATE NOT NULL,"
                                                  FILES_TIME " TIME NOT NULL,"
                                                  FILES_POSITION " DOUBLE NOT NULL,"
                                                  FILES_DELETED " BOOL FALSE"
                                                  ");";
    if(!query.exec(str)){
        qDebug() << "DataBase: error of create " << FILES;
        qDebug() << query.lastError().text();
        return false;
      }
    return true;
  }

  bool DataBase::createTableFFX()
  {
    // table FFX
    QString str = "CREATE TABLE " FFX " ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      FFX_DIAMETER " DOUBLE NOT NULL,"
                                      FFX_DIAMSQ " DOUBLE NOT NULL,"
                                      FFX_THICKNESS " DOUBLE NOT NULL,"
                                      FFX_WIDTH " DOUBLE NOT NULL,"
                                      FFX_TAGNAME " VARCHAR(255) NOT NULL,"
                                      FFX_ORSV " DOUBLE NOT NULL"
                                      ");";
    QSqlQuery query;
   if(!query.exec(str)){
        qDebug() << "DataBase: error of create " << FFX;
        qDebug() << query.lastError().text();
        return false;
     }
   return true;
  }

  bool DataBase::createTableFPSQ()
  {
    // table FPSQ
    QString str = "CREATE TABLE " FPSQ " ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      FPSQ_DIAMETER " DOUBLE NOT NULL,"
                                      FPSQ_TAGNAME " VARCHAR(255) NOT NULL,"
                                      FPSQ_ORDIAM " DOUBLE NOT NULL,"
                                      FPSQ_PRDIAM " DOUBLE NOT NULL,"
                                      FPSQ_ORSV " DOUBLE NOT NULL,"
                                      FPSQ_ENDIAM " DOUBLE NOT NULL"
                                      ");";
    QSqlQuery query;
    if(!query.exec(str)){
        qDebug() << "DataBase: error of create " << FPSQ;
        qDebug() << query.lastError().text();
        return false;
      }
    return true;
  }

  bool DataBase::createTableFFXDiam()
  {
    // table FFXDIAM
    QString str = "CREATE TABLE " FFXDIAM " ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      FFXDIAM_TYPDATA " VARCHAR(255) NOT NULL,"
                                      FFXDIAM_TAGNAME " VARCHAR(255) NOT NULL,"
                                      FFXDIAM_ORDIAM " DOUBLE NOT NULL,"
                                      FFXDIAM_PRDIAM " DOUBLE NOT NULL,"
                                      FFXDIAM_ENDIAM " DOUBLE NOT NULL"
                                      ");";
    QSqlQuery query;
    if(!query.exec(str)){
        qDebug() << "DataBase: error of create " << FFXDIAM;
        qDebug() << query.lastError().text();
        return false;
      }
    return true;

  }

  bool DataBase::createTableSavedSV()
  {
    // table SAVED
    QString str = "CREATE TABLE " SAVED " ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      SAVED_TAGNAME " VARCHAR(255) NOT NULL,"
                                      SAVED_SV " DOUBLE NOT NULL,"
                                      ");";
    QSqlQuery query;
    if(!query.exec(str)){
        qDebug() << "DataBase: error of create " << FFXDIAM;
        qDebug() << query.lastError().text();
        return false;
      }
    return true;
  }

}

