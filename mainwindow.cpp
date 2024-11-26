#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <string>
#include <algorithm>
#include <QSpinBox>
#include <QSqlRecord>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <sstream>

MainWindow::MainWindow(QWidget *parent, handler::Control *ptrC)
  : QMainWindow(parent), ptrC_(ptrC), ptrUi_(new Ui::MainWindow)
{
  ptrUi_->setupUi(this); 

  createWidgetsForProdInfoTab();
  createWidgetsForFilesTab();
  createWidgetsForDiameterTab();
  createWidgetsForPositionTab();
  createWidgetsForImageTab();
  createWidgetsForPasslineTab();
  createWidgetsForSetupTab();

  // for test
  ptrUi_->tv_allData->setModel(ptrC_->GetStandTableModel());
  // Allow the selection of lines
  ptrUi_->tv_allData->setSelectionBehavior(QAbstractItemView::SelectRows);
  // Set selection mode, only one row in table
  ptrUi_->tv_allData->setSelectionMode(QAbstractItemView::SingleSelection);

  // Set the size of the columns by content
  ptrUi_->tv_allData->resizeColumnsToContents();
  ptrUi_->tv_allData->setEditTriggers(QAbstractItemView::NoEditTriggers);

  signalToSlot();
  show();
}
// ====================================================================================
MainWindow::~MainWindow()
{
  delete ptrUi_;
}
// ====================================================================================
void MainWindow::createWidgetsForProdInfoTab()
{
  createComboBoxDiameterTube();
  createComboBoxWallThickness();
  updateTableViewProductSize();  
}

void MainWindow::createComboBoxDiameterTube()
{
  QSqlQueryModel* qm = ptrC_->GetPtrProductSize()->GetModelDiamTube();
  ptrUi_->cb_Od_product->setModel(qm);
  ptrUi_->cb_Od_FpSq->setModel(qm);
  //update current diameter tube
  currentPipeDiameter = ptrC_->GetPtrProductSize()->GetDiamTube();
}

void MainWindow::createComboBoxWallThickness()
{
  QStringList lst;

  QSqlQueryModel* qm = ptrC_->GetPtrProductSize()->GetModelThickness();
  int row = qm->rowCount();
  for(int i = 0; i < row; ++i){
      lst.append(qm->record(i).value("Thickness").toString());
    }
  lst << "ELSE";

  // update current wall thickness
  currentWallThickness = lst.first();

  ptrUi_->cb_wall_thickness->addItems(lst);
  ptrUi_->cb_wall_thickness->setEditable(true);
}

void MainWindow::updateTableViewProductSize()
{
  QString dataDiamSQ = ptrC_->GetPtrProductSize()->GetDiamAtSQ(currentPipeDiameter);
  QString dataWidth = ptrC_->GetPtrProductSize()->GetWidth(currentPipeDiameter, currentWallThickness);
  //_____________________update Product Info
  ptrUi_->lbl_ProdSizeValue->setText(currentPipeDiameter);
  ptrUi_->lbl_ThickValue->setText(currentWallThickness);
  ptrUi_->lbl_DiamAtSQValue->setText(dataDiamSQ);
  ptrUi_->lbl_WidthValue->setText(dataWidth);

  //____________________update diameter at SQ
  QTableWidgetItem* ptwi_1 = 0;
  QTableWidgetItem* ptwi_3 = 0;
  // update diameter at SQ (column Input)
  ptwi_1 = new QTableWidgetItem(dataDiamSQ);
  ptwi_1->setFlags(ptwi_1->flags() & ~Qt::ItemIsSelectable);
  ptrUi_->tbl_product_size->setItem(0, 0, ptwi_1);
  // update diameter at SQ (column Default - not editable)
  ptwi_3 = new QTableWidgetItem(dataDiamSQ);
  ptwi_3->setFlags(ptwi_3->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable); // cell not editable
  ptrUi_->tbl_product_size->setItem(0, 1, ptwi_3);

  //_____________________update width strip
  QTableWidgetItem* ptwi_2 = 0;
  QTableWidgetItem* ptwi_4 = 0;
  // update strip width (column Input)
  ptwi_2 = new QTableWidgetItem(dataWidth);
  ptwi_2->setFlags(ptwi_2->flags() & ~Qt::ItemIsSelectable);
  ptrUi_->tbl_product_size->setItem(1, 0, ptwi_2);
  // update strip width (column Default - not editable)
  ptwi_4 = new QTableWidgetItem(dataWidth);
  ptwi_4->setFlags(ptwi_4->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable); // cell not editable and selectable
  ptrUi_->tbl_product_size->setItem(1, 1, ptwi_4);

  // set current diameter and wall thickness in ProductSize
  ptrC_->GetPtrProductSize()->SetCurrentProduct(currentPipeDiameter, currentWallThickness);

  //____________________create settings for press top roll

}
// ====================================================================================
void MainWindow::createWidgetsForFilesTab()
{
  updateTableViewFilesTab();
}

void MainWindow::updateTableViewFilesTab()
{
  // QSqlTableModel requeres to connect DB continuously
  QSqlQueryModel* qm = ptrC_->GetPtrFiles()->GetFilesQueryModel();

  QStringList nameColumns;
  nameColumns << trUtf8("id") << trUtf8("Диаметр") << trUtf8("Толщина")
              << trUtf8("Ширина") << trUtf8("Сообщение") << trUtf8("Дата")
              << trUtf8("Время") << trUtf8("Позиция") << trUtf8("IsDeleted");

  for(int i = 0, j = 0; i < qm->columnCount(); i++, j++){
          qm->setHeaderData(i,Qt::Horizontal,nameColumns[j]);
      }

  ptrUi_->tv_DB->setModel(qm); // We set the model on the TableView
  // IsDeleted true hide this row
  for(int nRow = 0; nRow < qm->rowCount(); ++nRow){
      QSqlRecord rec = qm->record(nRow);

      if(rec.value("IsDeleted").toBool()){
          ptrUi_->tv_DB->hideRow(nRow);
        }
    }
  ptrUi_->tv_DB->hideColumn(0); ; // Hide the column id Records
  ptrUi_->tv_DB->hideColumn(7); ; // Hide the column Position
  ptrUi_->tv_DB->hideColumn(8); ; // Hide the column IsDeleted

  // Allow the selection of lines
  ptrUi_->tv_DB->setSelectionBehavior(QAbstractItemView::SelectRows);
  // Set selection mode, only one row in table
  ptrUi_->tv_DB->setSelectionMode(QAbstractItemView::SingleSelection);

  // Set the size of the columns by content
  ptrUi_->tv_DB->resizeColumnsToContents();
  ptrUi_->tv_DB->setEditTriggers(QAbstractItemView::NoEditTriggers);

  ptrUi_->tv_DB->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
  ptrUi_->tv_DB->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
  ptrUi_->tv_DB->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
  ptrUi_->tv_DB->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
  ptrUi_->tv_DB->horizontalHeader()->resizeSection(5, 100);
  ptrUi_->tv_DB->horizontalHeader()->resizeSection(6, 100);
  ptrUi_->tv_DB->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
  ptrUi_->tv_DB->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
}
// ====================================================================================
void MainWindow::createWidgetsForDiameterTab()
{
  updateFPSQTableModel();
  createFFXDiamTableModel();
}

void MainWindow::updateFPSQTableModel()
{
  QSqlQueryModel* qm = ptrC_->GetPtrDmt()->GetFpSqDiamQueryModel(currentDiamFpSq_);

  QStringList nameColumns;
  nameColumns << trUtf8("id") << trUtf8("Диаметр трубы")
              << trUtf8("Имя валка") << trUtf8("Оригинальный\nдиаметр")
              << trUtf8("Текущий\nдиаметр") << trUtf8("Позиция валка")
              << trUtf8("Новый\nдиаметр");

  for(int i = 0, j = 0; i < qm->columnCount(); i++, j++){
          qm->setHeaderData(i,Qt::Horizontal,nameColumns[j]);
      }

  ptrUi_->tv_fpsq_diam->setModel(qm);
  ptrUi_->tv_fpsq_diam->hideColumn(0);
  ptrUi_->tv_fpsq_diam->hideColumn(1);
  ptrUi_->tv_fpsq_diam->hideColumn(5);
  ptrUi_->tv_fpsq_diam->resizeColumnsToContents();
  int oldWidth = ptrUi_->tv_fpsq_diam->columnWidth(2);
  ptrUi_->tv_fpsq_diam->setColumnWidth(2, oldWidth + 147);
}

void MainWindow::createFFXDiamTableModel()
{
  QSqlQueryModel* qm = ptrC_->GetPtrDmt()->GetFfxDiamQueryModel();

  QStringList nameColumns;
  nameColumns << trUtf8("id") << trUtf8("Type data")
              << trUtf8("Имя валка") << trUtf8("Оригинальный\nдиаметр")
              << trUtf8("Текущий\nдиаметр") << trUtf8("Новый\nдиаметр");

  for(int i = 0, j = 0; i < qm->columnCount(); i++, j++){
          qm->setHeaderData(i,Qt::Horizontal,nameColumns[j]);
      }

  ptrUi_->tv_ffx_diam->setModel(qm);
  ptrUi_->tv_ffx_diam->hideColumn(0);
  ptrUi_->tv_ffx_diam->hideColumn(1);
  ptrUi_->tv_ffx_diam->resizeColumnsToContents();
  int oldWidth = ptrUi_->tv_ffx_diam->columnWidth(2);
  ptrUi_->tv_ffx_diam->setColumnWidth(2, oldWidth + 9);
}
// ====================================================================================
void MainWindow::createWidgetsForPositionTab()
{
  createProxyModels();
}

void MainWindow::createProxyModels()
{
  // create proxy model for view filtred data
  // FGS
  ProxyModel* proxyModel_FGS = new ProxyModel(this, stm::First);
  proxyModel_FGS->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_FGS->setFilterWildcard("FGS");
  ptrUi_->tv_FGS->setModel(proxyModel_FGS);
  ptrUi_->tv_FGS->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_FGS->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_FGS, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_FGS, stm::Fourth, stm::Ninth);
  ptrUi_->tv_FGS->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // TT
  ProxyModel* proxyModel_TT = new ProxyModel(this, stm::First);
  proxyModel_TT->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_TT->setFilterWildcard("TT");
  ptrUi_->tv_TT->setModel(proxyModel_TT);
  ptrUi_->tv_TT->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_TT->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_TT, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_TT, stm::Fourth, stm::Ninth);
  ptrUi_->tv_TT->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // EG
  ProxyModel* proxyModel_EG = new ProxyModel(this, stm::First);
  proxyModel_EG->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_EG->setFilterWildcard("EG");
  ptrUi_->tv_EG->setModel(proxyModel_EG);
  ptrUi_->tv_EG->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_EG->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_EG, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_EG, stm::Fourth, stm::Ninth);
  ptrUi_->tv_EG->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // PR
  ProxyModel* proxyModel_PR = new ProxyModel(this, stm::First);
  proxyModel_PR->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_PR->setFilterWildcard("PR");
  ptrUi_->tv_PR->setModel(proxyModel_PR);
  ptrUi_->tv_PR->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_PR->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_PR, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_PR, stm::Fourth, stm::Ninth);
  ptrUi_->tv_PR->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // BD1_Top
  ProxyModel* BD1_Top_Model = new ProxyModel(this, stm::First);
  BD1_Top_Model->setSourceModel(ptrC_->GetStandTableModel());
  BD1_Top_Model->setFilterWildcard("BD1_Top");
  ptrUi_->tv_BD1_Top->setModel(BD1_Top_Model);
  ptrUi_->tv_BD1_Top->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_BD1_Top->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_BD1_Top, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_BD1_Top, stm::Fourth, stm::Ninth);
  ptrUi_->tv_BD1_Top->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // BD1_Bottom
  ProxyModel* BD1_Bottom_Model = new ProxyModel(this, stm::First);
  BD1_Bottom_Model->setSourceModel(ptrC_->GetStandTableModel());
  BD1_Bottom_Model->setFilterWildcard("BD1_Bottom");
  ptrUi_->tv_BD1_Bottom->setModel(BD1_Bottom_Model);
  ptrUi_->tv_BD1_Bottom->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_BD1_Bottom->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_BD1_Bottom, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_BD1_Bottom, stm::Fourth, stm::Ninth);
  ptrUi_->tv_BD1_Bottom->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // BD2_Top
  ProxyModel* BD2_Top_Model = new ProxyModel(this, stm::First, stm::Second);
  BD2_Top_Model->setSourceModel(ptrC_->GetStandTableModel());
  BD2_Top_Model->setFilterWildcard("BD2_Top");
  ptrUi_->tv_BD2_Top->setModel(BD2_Top_Model);
  ptrUi_->tv_BD2_Top->setItemDelegate(new SpinBoxDelegate(this, "BD"));
  ptrUi_->tv_BD2_Top->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_BD2_Top, stm::First, stm::Second);
  hideColumn(ptrUi_->tv_BD2_Top, stm::Third, stm::Ninth);
  ptrUi_->tv_BD2_Top->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // BD2_Bottom
  ProxyModel* BD2_Bottom_Model = new ProxyModel(this, stm::First, stm::Second);
  BD2_Bottom_Model->setSourceModel(ptrC_->GetStandTableModel());
  BD2_Bottom_Model->setFilterWildcard("BD2_Bottom");
  ptrUi_->tv_BD2_Bottom->setModel(BD2_Bottom_Model);
  ptrUi_->tv_BD2_Bottom->setItemDelegate(new SpinBoxDelegate(this, "BD"));
  ptrUi_->tv_BD2_Bottom->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_BD2_Bottom, stm::First, stm::Second);
  hideColumn(ptrUi_->tv_BD2_Bottom, stm::Third, stm::Ninth);
  ptrUi_->tv_BD2_Bottom->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // BD3_Top
  ProxyModel* BD3_Top_Model = new ProxyModel(this, stm::First, stm::Second);
  BD3_Top_Model->setSourceModel(ptrC_->GetStandTableModel());
  BD3_Top_Model->setFilterWildcard("BD3_Top");
  ptrUi_->tv_BD3_Top->setModel(BD2_Top_Model);
  ptrUi_->tv_BD3_Top->setItemDelegate(new SpinBoxDelegate(this, "BD"));
  ptrUi_->tv_BD3_Top->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_BD3_Top, stm::First, stm::Second);
  hideColumn(ptrUi_->tv_BD3_Top, stm::Third, stm::Ninth);
  ptrUi_->tv_BD3_Top->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // BD3_Bottom
  ProxyModel* BD3_Bottom_Model = new ProxyModel(this, stm::First, stm::Second);
  BD3_Bottom_Model->setSourceModel(ptrC_->GetStandTableModel());
  BD3_Bottom_Model->setFilterWildcard("BD3_Bottom");
  ptrUi_->tv_BD3_Bottom->setModel(BD3_Bottom_Model);
  ptrUi_->tv_BD3_Bottom->setItemDelegate(new SpinBoxDelegate(this, "BD"));
  ptrUi_->tv_BD3_Bottom->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_BD3_Bottom, stm::First, stm::Second);
  hideColumn(ptrUi_->tv_BD3_Bottom, stm::Third, stm::Ninth);
  ptrUi_->tv_BD3_Bottom->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // BD4_Top
  ProxyModel* BD4_Top_Model = new ProxyModel(this, stm::First, stm::Second);
  BD4_Top_Model->setSourceModel(ptrC_->GetStandTableModel());
  BD4_Top_Model->setFilterWildcard("BD4_Top");
  ptrUi_->tv_BD4_Top->setModel(BD4_Top_Model);
  ptrUi_->tv_BD4_Top->setItemDelegate(new SpinBoxDelegate(this, "BD"));
  ptrUi_->tv_BD4_Top->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_BD4_Top, stm::First, stm::Second);
  hideColumn(ptrUi_->tv_BD4_Top, stm::Third, stm::Ninth);
  ptrUi_->tv_BD4_Top->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // BD4_Bottom
  ProxyModel* BD4_Bottom_Model = new ProxyModel(this, stm::First, stm::Second);
  BD4_Bottom_Model->setSourceModel(ptrC_->GetStandTableModel());
  BD4_Bottom_Model->setFilterWildcard("BD4_Bottom");
  ptrUi_->tv_BD4_Bottom->setModel(BD4_Bottom_Model);
  ptrUi_->tv_BD4_Bottom->setItemDelegate(new SpinBoxDelegate(this, "BD"));
  ptrUi_->tv_BD4_Bottom->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_BD4_Bottom, stm::First, stm::Second);
  hideColumn(ptrUi_->tv_BD4_Bottom, stm::Third, stm::Ninth);
  ptrUi_->tv_BD4_Bottom->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // RVS
  ProxyModel* proxyModel_RVS = new ProxyModel(this, stm::First);
  proxyModel_RVS->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_RVS->setFilterWildcard("RVS");
  ptrUi_->tv_RVS->setModel(proxyModel_RVS);
  ptrUi_->tv_RVS->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_RVS->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_RVS, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_RVS, stm::Fourth, stm::Ninth);
  ptrUi_->tv_RVS->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // CL1
  ProxyModel* proxyModel_CL1 = new ProxyModel(this, stm::First);
  proxyModel_CL1->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_CL1->setFilterWildcard("CL1");
  ptrUi_->tv_CL1->setModel(proxyModel_CL1);
  ptrUi_->tv_CL1->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_CL1->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_CL1, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_CL1, stm::Fourth, stm::Ninth);
  ptrUi_->tv_CL1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // CL2
  ProxyModel* proxyModel_CL2 = new ProxyModel(this, stm::First);
  proxyModel_CL2->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_CL2->setFilterWildcard("CL2");
  ptrUi_->tv_CL2->setModel(proxyModel_CL2);
  ptrUi_->tv_CL2->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_CL2->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_CL2, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_CL2, stm::Fourth, stm::Ninth);
  ptrUi_->tv_CL2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // CL3
  ProxyModel* proxyModel_CL3 = new ProxyModel(this, stm::First);
  proxyModel_CL3->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_CL3->setFilterWildcard("CL3");
  ptrUi_->tv_CL3->setModel(proxyModel_CL3);
  ptrUi_->tv_CL3->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_CL3->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_CL3, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_CL3, stm::Fourth, stm::Ninth);
  ptrUi_->tv_CL3->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // FP1
  ProxyModel* FP1_Model = new ProxyModel(this, stm::First);
  FP1_Model->setSourceModel(ptrC_->GetStandTableModel());
  FP1_Model->setFilterWildcard("FP1");
  ptrUi_->tv_FP1->setModel(FP1_Model);
  ptrUi_->tv_FP1->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_FP1->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_FP1, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_FP1, stm::Fourth, stm::Ninth);
  ptrUi_->tv_FP1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // FP2
  ProxyModel* FP2_Model = new ProxyModel(this, stm::First, stm::Second);
  FP2_Model->setSourceModel(ptrC_->GetStandTableModel());
  FP2_Model->setFilterWildcard("FP2");
  ptrUi_->tv_FP2->setModel(FP2_Model);
  ptrUi_->tv_FP2->setItemDelegate(new SpinBoxDelegate(this, "FP"));
  ptrUi_->tv_FP2->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_FP2, stm::First, stm::Second);
  hideColumn(ptrUi_->tv_FP2, stm::Third, stm::Ninth);
  ptrUi_->tv_FP2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // FP3
  ProxyModel* FP3_Model = new ProxyModel(this, stm::First, stm::Second);
  FP3_Model->setSourceModel(ptrC_->GetStandTableModel());
  FP3_Model->setFilterWildcard("FP3");
  ptrUi_->tv_FP3->setModel(FP3_Model);
  ptrUi_->tv_FP3->setItemDelegate(new SpinBoxDelegate(this, "FP"));
  ptrUi_->tv_FP3->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_FP3, stm::First, stm::Second);
  hideColumn(ptrUi_->tv_FP3, stm::Third, stm::Ninth);
  ptrUi_->tv_FP3->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // SQ
  ProxyModel* proxyModel_SQ = new ProxyModel(this, stm::First);
  proxyModel_SQ->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_SQ->setFilterWildcard("SQ");
  ptrUi_->tv_SQ->setModel(proxyModel_SQ);
  ptrUi_->tv_SQ->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_SQ->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_SQ, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_SQ, stm::Fourth, stm::Ninth);
  ptrUi_->tv_SQ->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // IBS
  ProxyModel* proxyModel_IBS = new ProxyModel(this, stm::First);
  proxyModel_IBS->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_IBS->setFilterWildcard("IBS");
  ptrUi_->tv_IBS->setModel(proxyModel_IBS);
  ptrUi_->tv_IBS->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_IBS->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_IBS, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_IBS, stm::Fourth, stm::Ninth);
  ptrUi_->tv_IBS->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // SG
  ProxyModel* proxyModel_SG = new ProxyModel(this, stm::First);
  proxyModel_SG->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_SG->setFilterWildcard("SG");
  ptrUi_->tv_SG->setModel(proxyModel_SG);
  ptrUi_->tv_SG->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_SG->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_SG, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_SG, stm::Fourth, stm::Ninth);
  ptrUi_->tv_SG->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // PS1
  ProxyModel* proxyModel_PS1 = new ProxyModel(this, stm::First);
  proxyModel_PS1->setSourceModel(ptrC_->GetStandTableModel());
  proxyModel_PS1->setFilterWildcard("PS1");
  ptrUi_->tv_PS1->setModel(proxyModel_PS1);
  ptrUi_->tv_PS1->setItemDelegate(new SpinBoxDelegate(this));
  ptrUi_->tv_PS1->setSelectionMode(QAbstractItemView::NoSelection);
  horHeaderResize(ptrUi_->tv_PS1, stm::First, stm::Third);
  hideColumn(ptrUi_->tv_PS1, stm::Fourth, stm::Ninth);
  ptrUi_->tv_PS1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
// ====================================================================================
void MainWindow::createWidgetsForImageTab()
{
  QGraphicsScene* scene = new QGraphicsScene(QRectF(-100, -100, 300, 300));

  ptrUi_->graphicsView->setScene(scene);
  QGraphicsRectItem* pRectItemBlue = scene->addRect(QRectF(100, 100, 120, 120),
                                                QPen(Qt::blue)
                                                );
  QGraphicsRectItem* pRectItemRed = scene->addRect(QRectF(-30, -30, 120, 120),
                                                QPen(Qt::red)
                                                );
  pRectItemBlue->setFlags(QGraphicsItem::ItemIsMovable);
  pRectItemRed->setFlags(QGraphicsItem::ItemIsMovable);

  QGraphicsLineItem* pLineItem = scene->addLine(QLineF(-10, -10, -80, -80), QPen(Qt::red, 2));
  QGraphicsTextItem* pTextItem = scene->addText("Child");
  pTextItem->setTransform(QTransform().shear(-0.5, 0.0), true);
  pTextItem->setParentItem((pLineItem));

  if(pRectItemBlue->collidesWithItem(pRectItemRed)){
      QMessageBox::information(0, "Information", "Collision discovered");
    }
  ptrUi_->graphicsView->show();
}
// ====================================================================================
void MainWindow::createWidgetsForPasslineTab()
{

}
// ====================================================================================
void MainWindow::createWidgetsForSetupTab()
{

}
// ====================================================================================
void MainWindow::signalToSlot()
{  
  connect(ptrUi_->m_pcmdSendSV, SIGNAL(clicked(bool)), ptrC_, SLOT(slotSendSv()));
  connect(ptrC_, SIGNAL(sigDebugMode()), SLOT(slotDebugMode()));

  connect(ptrUi_->m_pcmdCalcProdSize, SIGNAL(clicked(bool)),
         ptrC_, SLOT(slotCalcFromProductSize()));
}
// ====================================================================================
void MainWindow::slotDebugMode()
{
  if(ptrUi_->lbl_debug->isHidden()){
      ptrUi_->lbl_debug->show();
    }
  else{
      ptrUi_->lbl_debug->hide();
    }
}
// ====================================================================================
// Delegate to change sv
SpinBoxDelegate::SpinBoxDelegate(QObject *parent, const QString &nameStand)
  : QStyledItemDelegate(parent), m_nameStand(nameStand)
{

}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
  QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
  editor->setMinimum(-9999.999);
  editor->setMaximum(9999.999);
  editor->setDecimals(3);

  if(index.column() == stm::Third
     || (m_nameStand == "BD" && index.column() == stm::Second)
     || (m_nameStand == "FP" && index.column() == stm::Second)){
      return editor;
    }
  return nullptr;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  double value = index.model()->data(index, Qt::DisplayRole).toDouble();

  QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
  spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
  QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
  spinBox->interpretText();
  double value = spinBox->value();

  model->setData(index, value, Qt::EditRole);
}
// ====================================================================================
// Proxy model
ProxyModel::ProxyModel(QObject *parent, const int numCol_1, const int numCol_2)
  : QSortFilterProxyModel(parent), m_numCol_1(numCol_1), m_numCol_2(numCol_2){}

bool ProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{

      if(m_numCol_1 != source_column && m_numCol_2 != source_column){
          return true;
        }

      return false;
}
// ====================================================================================
void MainWindow::on_cb_Od_product_activated(const QString &arg1)
{
  qDebug() << "Current pipe diameter: " << arg1;
  currentPipeDiameter = arg1;
  updateTableViewProductSize();
}
// ====================================================================================
void MainWindow::on_cb_wall_thickness_activated(const QString &arg1)
{
  QString str = "Current wall thickness: " + arg1;
  //int n = QMessageBox::information(0, "Information", str);
  qDebug() << "Current wall thickness: " << arg1;
  currentWallThickness = arg1;  
  updateTableViewProductSize();
}
// ====================================================================================
void MainWindow::on_m_pcmdCalcProdSize_clicked()
{
  if(ptrUi_->tabWidget->count() > 1){
      ptrUi_->tabWidget->setCurrentIndex((ptrUi_->tabWidget->currentIndex() + 3)
                                         % ptrUi_->tabWidget->count());
    }
}
// ====================================================================================
SaveReceiptDialog::SaveReceiptDialog(QWidget *pwgt, const QVariantList& data)
  : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
  m_ptxtNote = new QLineEdit;

  QLabel* plblDiameter = new QLabel("Diameter, mm");
  QString strDiam = data[0].toString();
  QLabel* plblValueDiameter = new QLabel(strDiam);

  QLabel* plblThickness = new QLabel("Thikcness, mm");
  QString strThick = data[1].toString();
  QLabel* plblValueThickness = new QLabel(strThick);

  QLabel* plblDiamAtSq = new QLabel("Diameter at SQ, mm");
  QString strDiamAtSQ = data[2].toString();
  QLabel* plblValueDiamAtSq = new QLabel(strDiamAtSQ);

  QLabel* plblWidth = new QLabel("Width, mm");
  QString strWidth = data[3].toString();
  QLabel* plblValueWidth = new QLabel(strWidth);

  QLabel* plblData = new QLabel("Data");
  QString strDate = data[4].toString();
  QLabel* plblValueData = new QLabel(strDate);

  QLabel* plblNote = new QLabel("&Note");

  plblNote->setBuddy(m_ptxtNote);

  QPushButton* pcmdOk = new QPushButton("&Ok");
  QPushButton* pcmdCancel = new QPushButton("&Cancel");

  connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
  connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

  QGridLayout* ptopLayout = new QGridLayout;
  ptopLayout->addWidget(plblDiameter, 0, 0);
  ptopLayout->addWidget(plblValueDiameter, 0, 1);
  ptopLayout->addWidget(plblThickness, 1, 0);
  ptopLayout->addWidget(plblValueThickness, 1, 1);
  ptopLayout->addWidget(plblDiamAtSq, 2, 0);
  ptopLayout->addWidget(plblValueDiamAtSq, 2, 1);
  ptopLayout->addWidget(plblWidth, 3, 0);
  ptopLayout->addWidget(plblValueWidth, 3, 1);
  ptopLayout->addWidget(plblData, 4, 0);
  ptopLayout->addWidget(plblValueData, 4, 1);
  ptopLayout->addWidget(plblNote, 5, 0);
  ptopLayout->addWidget(m_ptxtNote, 5, 1);
  ptopLayout->addWidget(pcmdOk, 6, 0);
  ptopLayout->addWidget(pcmdCancel, 6, 1);
  setLayout(ptopLayout);
}

QString SaveReceiptDialog::Note() const
{
  return m_ptxtNote->text();
}
// ====================================================================================
void MainWindow::on_m_pcmdSave_clicked() // to transfer code to control.cpp???
{
  // prepare data for Save dialog and send to FilesModel
  QString strDiam = currentPipeDiameter;
  double diameter = strDiam.toDouble();
  QString strThickness = currentWallThickness;
  double thickness = strThickness.toDouble();
  QString strWidth = ptrC_->GetPtrProductSize()->GetWidth(currentPipeDiameter,
                                                          currentWallThickness);

  double width = strWidth.toDouble();
  QString diamAtSq = ptrC_->GetPtrProductSize()->GetDiamAtSQ(currentPipeDiameter);
  QDate date = QDate::currentDate();
  QString strDate = date.toString();
  QTime time = QTime::currentTime();
  QString position;

  std::ostringstream ostr;
  // read NameTag and PV from stm model
  stm::StandTableModel* myModel = ptrC_->GetStandTableModel();
  for(int i = 0; i < myModel->rowCount(QModelIndex()); ++i){
      QModelIndex nameTag = myModel->index(i, stm::First);
      QModelIndex pv = myModel->index(i, stm::Third);

      ostr << nameTag.data().toString().toStdString() << ":" <<
              pv.data().toDouble() << ",";
    }
  std::string ret = ostr.str();
  ret = ret.substr(0, ret.size() - 1);
  position = QString::fromStdString(ret);


  // prepare data for dialog view
  QVariantList dataDialog;

  dataDialog.append(currentPipeDiameter);
  dataDialog.append(currentWallThickness);
  dataDialog.append(diamAtSq);
  dataDialog.append(strWidth);
  dataDialog.append(strDate);

  SaveReceiptDialog* pSaveReceipt = new SaveReceiptDialog(this, dataDialog);
  if(pSaveReceipt->exec() == QDialog::Accepted){
      // prepare data for db
      QVariantList dataDB;

      dataDB.append(diameter);
      dataDB.append(thickness);
      dataDB.append(width);
      dataDB.append(pSaveReceipt->Note());
      dataDB.append(date);
      dataDB.append(time);
      dataDB.append(position);

      ptrC_->GetPtrFiles()->SaveReceiptToFilesModel(dataDB);
      updateTableViewFilesTab();

      QMessageBox::information(0, "Information", "Data saved");
    }
  delete pSaveReceipt;
}
// ====================================================================================
void MainWindow::on_m_pcmdRegeneration_clicked()
{
    updateTableViewFilesTab();
    QMessageBox::information(0, "Information", "Data updated");
}
// ====================================================================================
void MainWindow::on_m_pcmdDelete_clicked()
{

  QModelIndexList selectedIndex = ptrUi_->tv_DB->selectionModel()->selection().indexes();

  if(!(selectedIndex.empty())){
      QMessageBox* pmbx = new QMessageBox(QMessageBox::Question,
                                         "Manage data",
                                         "Do you want delete this note?",
                                         QMessageBox::Cancel | QMessageBox::Yes);

        int n = pmbx->exec();

        if(n == QMessageBox::Yes){


            QModelIndex index_1 = selectedIndex.at(0).siblingAtColumn(fls::First);
            if(ptrC_->GetPtrFiles()->RemoveReceiptFromFilesModel(index_1.data().toInt())){
                updateTableViewFilesTab();
                //QMessageBox::information(0, "Information", "Note has deleted!");
              }
            else{
                QMessageBox::information(0, "Information", "Note hasn't deleted!'\n'Take a look logs.");
              }

          }
    }
  else{
      QMessageBox::information(0, "information", "Select note, please!");
    }

}
// ====================================================================================
void MainWindow::on_m_pcmdLoad_clicked()
{
    QModelIndexList selectedIndex = ptrUi_->tv_DB->selectionModel()->selection().indexes();

    if(!(selectedIndex.empty())){
        QMessageBox* pmbx = new QMessageBox(QMessageBox::Question,
                                           "Manage data",
                                           "Do you want load this receipt?",
                                           QMessageBox::Cancel | QMessageBox::Yes);

        int n = pmbx->exec();

        if(n == QMessageBox::Yes){
            process::Reader rd;
            QModelIndex index_8 = selectedIndex.at(0).siblingAtColumn(fls::Eighth);
            QString position = index_8.data().toString();

            if(ptrC_->GetStandTableModel()->UpdateFromDbSV(rd.readStringToSV(position))){
                QMessageBox::information(0, "Information", "Positions of receipt was load to setup value!");
              }
            else{
                QMessageBox::information(0, "Information", "Positions haven't loaded!\nTake a look logs.");
              }

          }
      }
    else{
        QMessageBox::information(0, "information", "Select settings, please!");
      }
}
// ====================================================================================
void MainWindow::on_m_pcmdRestore_clicked()
{
  QMessageBox* pmbx = new QMessageBox(QMessageBox::Question,
                                      "Manage Data",
                                      "Do you want restore all data?",
                                      QMessageBox::Cancel | QMessageBox::Yes);
  int n = pmbx->exec();
  if(n == QMessageBox::Yes){

    if(ptrC_->GetPtrFiles()->RestoreReceiptInFilesModel()){
        updateTableViewFilesTab();
        QMessageBox::information(0, "Information", "Notes restored");
      }
    }

}
// ====================================================================================
void MainWindow::on_m_pcmdHomePosition_clicked()
{
  QMessageBox* pmbx = new QMessageBox(QMessageBox::Question,
                                      "Manage Data",
                                      "Do you want set home position?",
                                      QMessageBox::Cancel | QMessageBox::Yes);
  int n = pmbx->exec();
  if(n == QMessageBox::Yes){
      // to add return status from function
    ptrC_->GetStandTableModel()->SetHomePosToPV();
    QMessageBox::information(0, "Information", "Home position loaded");
    }
}
// ====================================================================================
void MainWindow::on_m_pcmdCopyPVtoSV_clicked()
{
  QMessageBox* pmbx = new QMessageBox(QMessageBox::Question,
                                      "Manage Data",
                                      "Do you want copy present value to setup value?",
                                      QMessageBox::Cancel | QMessageBox::Yes);
  int n = pmbx->exec();
  if(n == QMessageBox::Yes){
       // to add return status from function
    ptrC_->GetStandTableModel()->CopyPvToSv();
    QMessageBox::information(0, "Information", "Copying is done");
    }
}
// ====================================================================================
void MainWindow::on_cb_Od_FpSq_activated(const QString &arg1)
{
    currentDiamFpSq_ = arg1;
    updateFPSQTableModel();
}
// ====================================================================================
