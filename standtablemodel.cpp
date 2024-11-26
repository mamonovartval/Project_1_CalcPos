#include "standtablemodel.h"
#include "process.h"

namespace stm {
  StandTableModel::StandTableModel(QObject *pobj)
    : QAbstractTableModel(pobj)
  {
    QString file = "/home.json";
    process::Reader rd;

    // set name of DB for read data
    nameDB_ = process::detail::db_test;

    // set rows and columns
    m_nRows = 70;
    m_nColumns = 10;

    std::vector<cmn::Device> tags = std::get<std::vector<cmn::Device>>(rd.Read(file));

    if(tags.empty()){
        qDebug() << "No tags in file";
      }

    for(int row = 0; row < tags.size(); ++row){
        // create index for first column - tag name, using for reading data from PLC, don't change
        QModelIndex index_1 = this->index(row, First);
        // create index for second column - name axis
        QModelIndex index_2 = this->index(row, Second);
        // create index for third column - p.v.
        QModelIndex index_3 = this->index(row, Third);
        // create index for fourth column - s.v.
        QModelIndex index_4 = this->index(row, Fourth);
        // create index for fifth column - base position
        QModelIndex index_5 = this->index(row, Fifth);
        // create index for fifth column - min limit
        QModelIndex index_6 = this->index(row, Sixth);
        // create index for fifth column - max limit
        QModelIndex index_7 = this->index(row, Seventh);
        // create index for fifth column - min calc limit
        QModelIndex index_8 = this->index(row, Eighth);
        // create index for fifth column - max calc limit
        QModelIndex index_9 = this->index(row, Ninth);
        // create index for tenth column - calc s.v.
        QModelIndex index_10 = this->index(row, Tenth);

        // fill m_hash
        m_hash[index_1] = tags[row].nameTag;
        m_hash[index_2] = tags[row].nameAxis;
        m_hash[index_3] = -1234.567;
        m_hash[index_4] = -1234.567;
        m_hash[index_5] = tags[row].posLim.base;
        m_hash[index_6] = tags[row].posLim.min;
        m_hash[index_7] = tags[row].posLim.max;
        m_hash[index_8] = 0.0;
        m_hash[index_9] = 0.0;
        m_hash[index_10] = 0.0;
      }    
  }

  QVariant StandTableModel::data(const QModelIndex &index, int role) const
  {
    if (!index.isValid()) {
        return QVariant();
    }

    if(role == Qt::DisplayRole){
        return m_hash.value(index, QVariant());
      }
    else if(role == Qt::BackgroundRole && index.column() == Second){
        return QColor(221, 221, 221);
      }
    else if(role == Qt::ForegroundRole && index.column() == Third){
        return QBrush(Qt::red);
      }
    else if(role == Qt::ForegroundRole && index.column() == Fourth){
        return QBrush(Qt::blue);
      }
    else{
        return QVariant();
      }
  }

  bool StandTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
  {
    if (index.isValid() && role == Qt::EditRole) {
        m_hash[index] = value;
        emit dataChanged(index, index);
        return true;
    }
    return false;
  }

  int StandTableModel::rowCount(const QModelIndex &parent) const
  {
    return m_nRows;
  }

  int StandTableModel::columnCount(const QModelIndex &parent) const
  {
    return m_nColumns;
  }

  Qt::ItemFlags StandTableModel::flags(const QModelIndex &index) const
  {
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsEditable)
                           : flags;
  }

  QVariant StandTableModel::headerData(int section, Qt::Orientation orientation, int role) const
  {
    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            switch (section) {
              case Second:
                return QString("Name axis");
                break;
              case Third:
                return QString("P.V.");
                break;
              case Fourth:
                return QString("S.V.");
                break;
              case Fifth:
                return QString("Base position");
                break;
              case Sixth:
                return QString("Min limit");
                break;
              case Seventh:
                return QString("Max limit");
                break;
              case Eighth:
                return QString("Min calc limit");
                break;
              case Ninth:
                return QString("Max calc limit");
                break;
              case Tenth:
                return QString("Calc S.V.");
                break;
              default:
                break;
              }
          }
      }
    return QVariant();

  }

  const QString &StandTableModel::GetNameDB() const
  {
    return nameDB_;
  }

  void StandTableModel::CopyPvToSv()
  {
    for(int i = 0; i < m_nRows; ++i){
        QModelIndex index_3 = this->index(i, Third);
        // data from pv column
        QVariant pv = index_3.data();

        QModelIndex index_4 = this->index(i, Fourth);
        // set data to sv column
        setData(index_4, pv, Qt::EditRole);
      }
  }

  void StandTableModel::SetHomePosToPV()
  {
    for(int i = 0; i < m_nRows; ++i){
        QModelIndex index_5 = this->index(i, Fifth);
        // data from pv column
        QVariant base = index_5.data();

        QModelIndex index_3 = this->index(i, Third);
        // set data to sv column
        setData(index_3, base, Qt::EditRole);
      }

    CopyPvToSv();
  }

  bool StandTableModel::UpdateCalcSV(const QHash<QString, double> &tableSV)
  {
    bool result = true;
    for(int row = 0; row < m_nRows; ++row){
        QModelIndex index_1 = this->index(row, First);
        QModelIndex index_10 = this->index(row, Tenth);

        QString key = index_1.data().toString();
        if(tableSV.contains(key)){
            setData(index_10, tableSV.value(key), Qt::EditRole);
          }
        else{
            qDebug() << key << " not present in TableSV.";
            qDebug() << " Make to right calculation";

            result = false;
          }
      }

    copyCalcSvToSv();

    return result;
  }

  bool StandTableModel::UpdateFromDbSV(const QHash<QString, double> &tableSV)
  {
    bool result = true;
    for(int row = 0; row < m_nRows; ++row){
        QModelIndex index_1 = this->index(row, First);
        QModelIndex index_4 = this->index(row, Fourth);

        QString key = index_1.data().toString();        
        if(tableSV.contains(key)){
            setData(index_4, tableSV.value(key), Qt::EditRole);
          }
        else{
            qDebug() << key << " not present in Table.";

            result = false;
          }
      }
    return result;
  }

  void StandTableModel::copyCalcSvToSv()
  {
    for(int i = 0; i < m_nRows; ++i){
        QModelIndex index_10 = this->index(i, Tenth);
        // data from pv column
        QVariant calcSv = index_10.data();

        QModelIndex index_4 = this->index(i, Fourth);
        // set data to sv column
        setData(index_4, calcSv, Qt::EditRole);
      }
  }
}





