#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "control.h"
#include <QTableWidget>
#include <QItemDelegate>
#include <QSqlTableModel>
#include <QDialog>

#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>

QT_BEGIN_NAMESPACE
namespace Ui
{
  class MainWindow;
  class SpinBoxDelegate;  
  class ProxyModel;
  class SaveReceiptDialog;

  const int heightRow = 85;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT  

public:
  MainWindow(QWidget *parent = nullptr, handler::Control* ptrC = nullptr);

  ~MainWindow();

signals:
  void Update();   

public slots:
  void slotDebugMode();

private slots:
  //___________Tab Product Info
  void on_cb_Od_product_activated(const QString &arg1);
  void on_cb_wall_thickness_activated(const QString &arg1);    
  void on_m_pcmdCalcProdSize_clicked();
  void on_m_pcmdHomePosition_clicked();

  //___________Tab File
  void on_m_pcmdSave_clicked();
  void on_m_pcmdRegeneration_clicked();
  void on_m_pcmdDelete_clicked();
  void on_m_pcmdLoad_clicked();
  void on_m_pcmdRestore_clicked();

  //___________Tab Position
  void on_m_pcmdCopyPVtoSV_clicked(); 

  void on_cb_Od_FpSq_activated(const QString &arg1);

private:
  Ui::MainWindow *ptrUi_;
  handler::Control* ptrC_;

  QString currentPipeDiameter;
  QString currentWallThickness; 

  QString currentDiamFpSq_ = "114.3";

  //___________Tab Product Info
  void createWidgetsForProdInfoTab();
  void createComboBoxDiameterTube();
  void createComboBoxWallThickness();
  void updateTableViewProductSize();

  //___________Tab File
  void createWidgetsForFilesTab();
  void updateTableViewFilesTab();

  //___________Tab Diameter
  void createWidgetsForDiameterTab();
  void updateFPSQTableModel();
  void createFFXDiamTableModel();



  //___________Tab Position
  void createWidgetsForPositionTab();
  void createProxyModels();
// ====================================================================================
  template<typename T>
  void hideColumn(T* widget, const int first, const int last){
    for(int i = first; i <= last; ++i){
        widget->hideColumn(i);
      }
  }
// ====================================================================================
  template<typename M>
  void horHeaderResize(M* widget, const int first, const int last){
    for(int i = first; i <= last; ++i){
        if(i == first){
            widget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
          }
        else{
            widget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
          }
      }
  }
// ====================================================================================


  //___________Tab Image
  void createWidgetsForImageTab();




  //___________Tab Pass Line
  void createWidgetsForPasslineTab();




  //___________Tab Setup
  void createWidgetsForSetupTab();

  void signalToSlot();  
};
// ====================================================================================
class SpinBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  SpinBoxDelegate(QObject *parent = 0, const QString& nameStand = "None");
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
  QString m_nameStand;
};
// ====================================================================================
class ProxyModel : public QSortFilterProxyModel{
  Q_OBJECT
public:
  ProxyModel(QObject *parent = 0, const int numCol_1 = -1, const int numCol_2 = -1);
  bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
private:
  int m_numCol_1;
  int m_numCol_2;
};
// ====================================================================================
class QLineEdit;
// ====================================================================================
class SaveReceiptDialog : public QDialog{
  Q_OBJECT
public:
  SaveReceiptDialog(QWidget* pwgt = 0, const QVariantList& data = QVariantList());
  QString Note() const;
private:
  QLineEdit* m_ptxtNote;
};
// ====================================================================================
#endif // MAINWINDOW_H
