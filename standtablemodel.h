#ifndef STANDTABLEMODEL_H
#define STANDTABLEMODEL_H
#include <QtWidgets>

/*
There are set values positon which coresponding initial("home") position regarding mechanical drawings from supplier.
And mechanical limit values for each axis stands.
*/

namespace  stm {
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

  class StandTableModel : public QAbstractTableModel
  {
    Q_OBJECT
  public:
    StandTableModel(QObject *pobj);
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void SetNameDB(const QString& name);
    const QString& GetNameDB() const;
    void CopyPvToSv();
    void SetHomePosToPV();

    bool UpdateCalcSV(const QHash<QString, double>& tableSV);
    bool UpdateFromDbSV(const QHash<QString, double>& tableSV);

  private:
    int m_nRows;
    int m_nColumns;
    QHash<QModelIndex, QVariant> m_hash;

    QString nameDB_ = "none";    

    bool verifyCalcSv(const QHash<QString, double>& data);
    void copyCalcSvToSv();

    //10.03.2023 to do common method for copy with specify index from and index to

  };
}
#endif // STANDTABLEMODEL_H
