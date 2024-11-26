#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#include "common.h"

/* Directive table names, fields, tables, and databases */
#define DATABASE_HOSTNAME       "FFXDataBase"
#define DATABASE_NAME           "Test.db"

// Name tables
#define FILES                   "Files"
#define FILES_DIAMETER          "Diameter"
#define FILES_THICKNESS         "Thickness"
#define FILES_WIDTH             "Width"
#define FILES_NOTE              "Note"
#define FILES_DATE              "Date"
#define FILES_TIME              "Time"
#define FILES_POSITION          "Position"
#define FILES_DELETED           "IsDeleted"

#define FPSQ                    "FPSQ"
#define FPSQ_DIAMETER           "Diameter_tube"
#define FPSQ_TAGNAME            "Name_axis"
#define FPSQ_ORDIAM             "Original_diameter"
#define FPSQ_PRDIAM             "Present_diameter"
#define FPSQ_ORSV               "Original_SV"
#define FPSQ_ENDIAM             "Entry_diameter"

#define FFXDIAM                 "FFXDIAM"
#define FFXDIAM_TYPDATA         "Type_data"
#define FFXDIAM_TAGNAME         "Name_axis"
#define FFXDIAM_ORDIAM          "Original_diameter"
#define FFXDIAM_PRDIAM          "Present_diameter"
#define FFXDIAM_ORSV            "Original_SV"
#define FFXDIAM_ENDIAM          "Entry_diameter"

#define FFX                     "FFX"
#define FFX_DIAMETER            "Diameter_tube"
#define FFX_DIAMSQ              "Diameter_at_SQ"
#define FFX_THICKNESS           "Thickness"
#define FFX_WIDTH               "Width"
#define FFX_TAGNAME             "Name_axis"
#define FFX_ORSV                "Original_SV"

#define SAVED                   "Saved"
#define SAVED_TAGNAME           "Name_axis"
#define SAVED_SV                "Saved_SV"
//#define TAGNAME                 "Name axis"
//#define ORDIAM                  "Original diameter"
//#define PRDIAM                  "Present diameter"
//#define ORSV                    "Original S.V."
//#define ENDAIM                  "Entry diameter"

/*
There is database model for manage settings position stands.
*/

namespace db {
  enum Status{
    CLOSE,
    OPEN,
    RESTORE
  };

  class DataBase : public QObject
  {
    Q_OBJECT
  public:
    DataBase(QObject* pobj = 0);
    QVariant ConnectToDataBase();
    void DisconnectFromDataBase();

    bool InsertIntoFFXTable(const QVariantList& data);
    bool InsertIntoFPSQTable(const QVariantList& data);
    bool InsertIntoFFXDiamTable(const QVariantList& data);
    bool InsertIntoSavedTable(const QVariantList& data);

    const QHash<QString, double> GetDataFromDB(const QString& nameTable,
                                               const cmn::Product& nameColumn);
    bool UpdateDataInDB(const QString& nameTable, const QHash<QString, double>& data);
    bool UploadDataFromDB(const QString& nameTable, QHash<QString, double>& data);

    const QSqlDatabase& GetDB();

  private:
    QSqlDatabase db_;


    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTables();
    void readDataBase();

    bool createTableFiles();
    bool createTableFFX();
    bool createTableFPSQ();
    bool createTableFFXDiam();
    bool createTableSavedSV(); // for restore data after switch on application
  };
}


#endif // DATABASE_H
