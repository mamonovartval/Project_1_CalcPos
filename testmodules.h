#ifndef TESTMODULES_H
#define TESTMODULES_H
//#include <QTest>
#include <cassert>
#include <algorithm>
#include <QSqlRecord>
#include <sstream>
#include <string>

#include "common.h"
#include "connection.h"
#include "plcdata.h"
#include "process.h"
#include "clientopc.h"
#include "mainwindow.h"
#include "standtablemodel.h"
#include "database.h"
#include <calculation.h>
#include "productsize.h"
#include "files.h"

/*
Tests are our everything!!!
*/


const double EPSILON = 0.00001;

void ChangeValue(stm::StandTableModel* stm, const int n, const QString& name){

  for(int i = 0; i < stm->rowCount(QModelIndex()); ++i){
      QModelIndex index_1 = stm->index(i, 0);
      // create full name format tag for read from PLC
      QString nameTag = index_1.data().toString();
      // create format tag for connection
      cmn::Variable fullNameTag(stm->GetNameDB(), nameTag);
      // update data inside container
      QVariant value(n);
      if(name == "pv"){
          QModelIndex index = stm->index(i, stm::Third);
          stm->setData(index, value, Qt::EditRole);
        }
      else if(name == "sv"){
          QModelIndex index = stm->index(i, stm::Fourth);
          stm->setData(index, value, Qt::EditRole);
        }
      else{
          return;
        }
  }
}

void TestConnection(){
//  client::Connection myConnect;
//
//  cmn::SettingOPC setup{"10.8.200.51", "4840"};
//  myConnect.SetSetting(setup);
//  assert("10.8.200.51" == myConnect.GetSetting().ip_adress);
//  assert("4840" == myConnect.GetSetting().port_adress);
//
//  myConnect.Connect();
//  assert(myConnect.GetStatus() == UA_STATUSCODE_GOOD);
//
//  myConnect.Disconnect();
//  assert(myConnect.GetStatus() == UA_STATUSCODE_GOOD);
//
//  qDebug() << "Class Connection test is done";
}

void TestPLCData(){
//  client::Connection myConnect;
//  cmn::SettingOPC setup{"10.8.200.51", "4840"};
//  myConnect.SetSetting(setup);
//  myConnect.Connect();
//
///* Read attribute */
//  // parameter
//  int node = 3; // node of plc
//  QString db_name = "Test_for_FFX";
//  QString tag_name = "Spare_2025";
//  cmn::Variable var(db_name, tag_name);
//
//  client::PlcData myPlc;
//  myPlc.ReadData(myConnect.GetClient(), node, var.GetVariable());
//  assert(UA_STATUSCODE_GOOD == myPlc.IsRead());
//
//
//  /* Write node attribute
// Take in account that db should be able to write data. It means that db has not rewrite data from another source*/
//  UA_Int32 value = 1000;
//  myPlc.WriteData(myConnect.GetClient(), node, var.GetVariable(), value);
//  assert(UA_STATUSCODE_GOOD == myPlc.IsWrite());
//  myPlc.ReadData(myConnect.GetClient(), node, var.GetVariable());
//  assert(UA_STATUSCODE_GOOD == myPlc.IsWrite());
//  myConnect.Disconnect();
//
//  qDebug() << "Class PlcData test is done";
}

void TestProcess(){
  QString file = "/home.json";
  process::Reader rd;

  // test ReadJson
  /*QJsonDocument doc = rd.readJSON(file);
  assert(doc.isEmpty() == false);*/

  // test SeparateTag
  /*{
  QString qName = "PR1_Top_Roll_Height";
  std::string str = qName.toStdString();

  cmn::Device dev = rd.separateTag(str);
  assert(dev.nameStand == "PR1");
  assert(dev.nameAxis == "Top_Roll_Height");
  }*/

  // test readSetting
  /*QJsonObject obj = doc.object();
  {
    cmn::SettingOPC sett = rd.readSetting(obj);
    assert("10.8.200.51" == sett.ip_adress);
    assert("4840" == sett.port_adress);
  }*/

  // test readTags
  /*{
    std::vector<cmn::Stand> tags = rd.readTags(obj, process::detail::db_test);
    assert(tags.empty() == false);
    assert(tags.size() == 23);
    assert(tags[0].nameStand == "PR1");
    assert(tags[0].axises[0].nameAxis == "Top_Roll_Height");
    assert(tags[0].axises[0].value == 0);
    assert(tags[22].nameStand == "PS1");
    assert(tags[0].axises[0].nameAxis == "Top_Roll_Height");
    assert(tags[0].axises[0].value == 0);
  }*/

  // test ReadData
  {
    //ffx::DataManFFX dm;
    //client::ClientOPC clnt;
    //rd.ReadData(file, dm, clnt);
    // verify data inside data manager FFX after initialization
 //  assert(dm.GetPV("PR1").size() == 1);
 //  assert(dm.GetPV("PR1").begin()->value == 0);
 //  assert(dm.GetPV("PR1").begin()->nameAxis == "Top_Roll_Height");
 //
 //   // verify data inside client OPC after initialization
 //   assert(clnt.GetSetting().port_adress == "4840");
 //   assert(clnt.GetSetting().ip_adress == "10.8.200.51");
 //   assert(clnt.GetStatus() == UA_STATUSCODE_UNCERTAIN); // because it's not connect

    //QSet<cmn::Stand>* stands;
    //rd.ReadData(file, stands);
    //qDebug() << "Class Reader test is done";
  }

  // test method Read for stand model
  {
    std::vector<cmn::Device> tags = std::get<std::vector<cmn::Device>>(rd.Read(file));
    assert(tags[0].nameTag == "BD1_Bottom_Roll_Height");
    assert(tags[0].nameStand == "BD1");
    assert(tags[0].nameAxis == "Bottom Roll Height");
    assert(tags[0].posLim.base == -92.0);
    assert(tags[0].posLim.min == -253.0);
    assert(tags[0].posLim.max == -53.0);
  }

  // test method Read for FPSQ model
  file = "/FPSQ.json";
  {
    std::vector<cmn::RollDiam> tags = std::get<std::vector<cmn::RollDiam>>(rd.Read(file));

    assert(tags[0].product.diameter == 114.3);
    assert(tags[0].nameTag == "CL1_Side_Roll_DS_Width");
    assert(tags[0].presDiam == 329.78);
    assert(tags[0].origDiam == 329.78);
    assert(tags[0].origSV == 990.62);
    assert(tags[0].calcSV == 0.0);
  }

  // test method Read for FFXDiam model
  file = "/FFX_diam.json";
  {
    std::vector<cmn::ChangeDiam> tags = std::get<std::vector<cmn::ChangeDiam>>(rd.Read(file));

    assert(tags[15].typeData == "Regrind data");
    assert(tags[15].nameTag == "BD1_Bottom_Liner");
    assert(tags[15].arr_origDiam[0] == 10);
    assert(tags[15].arr_origDiam[1] == 10);
  }

  // test method Read for FFX model
  file = "/FFX.json";
  {
    std::vector<cmn::RollDiam> tags = std::get<std::vector<cmn::RollDiam>>(rd.Read(file));

    assert(tags[0].product.diameter == 114.3);
    assert(tags[0].product.diameterAtSQ == 115.45);
    assert(tags[0].product.wallThickness == 10);
    assert(tags[0].product.stripsWidth == 346.7);
    assert(tags[0].nameTag == "BD1_Bottom_Roll_Height");
    assert(tags[0].origSV == -92);
  }

  // test read QString position from DB to SV
  {
    handler::Control* c = new handler::Control;
    stm::StandTableModel* myModel = c->GetStandTableModel();
    QString position;

    std::ostringstream ostr;
    // read NameTag and PV from stm model
    for(int i = 0; i < myModel->rowCount(QModelIndex()); ++i){
        QModelIndex nameTag = myModel->index(i, stm::First);
        QModelIndex pv = myModel->index(i, stm::Fourth);

        ostr << nameTag.data().toString().toStdString() << ":" <<
                pv.data().toDouble() << ",";
      }
    std::string ret = ostr.str();
    ret = ret.substr(0, ret.size() - 1);
    position = QString::fromStdString(ret);
    assert(true == myModel->UpdateFromDbSV(rd.readStringToSV(position)));
  }

  qDebug() << "Class Reader test is done";
}

void TestStandTableModel(){
  stm::StandTableModel* ptrStm = new stm::StandTableModel(nullptr);

  // init data in pv_
  {
    ChangeValue(ptrStm, 100, "pv");
  }
  //copy pv to sv data
  {
    ptrStm->CopyPvToSv();

    // test update sv
    for(int i = 0; i < ptrStm->rowCount(QModelIndex()); ++i){
        QModelIndex index_3 = ptrStm->index(i, stm::Third);
        QModelIndex index_4 = ptrStm->index(i, stm::Fourth);

        assert(index_3.data().toInt() == index_4.data().toInt());
      }
  }

  // update CalcSv
  {
    ps::ProductSize *ptrPs = new ps::ProductSize(nullptr);

    assert(true == ptrStm->UpdateCalcSV(ptrPs->GetTableSV()));
  }

  qDebug() << "Class StandTableModel test is done";
}

void TestClientOPC(){
  QString file = "/data.json";
  process::Reader rd;

  stm::StandTableModel* stm = new stm::StandTableModel(nullptr);
  client::ClientOPC clnt;
  clnt.InitSetting(std::get<cmn::SettingOPC>(rd.Read(file)));

  assert("10.8.200.51" == clnt.GetSetting().ip_adress);
  assert("4840" == clnt.GetSetting().port_adress);

  clnt.ClientOn();
  assert(clnt.GetStatus() == UA_STATUSCODE_GOOD);

  clnt.ReadPV(stm);
  // test updated data in pv_
  {
    for(int i = 0; i < stm->rowCount(QModelIndex()); ++i){
        QModelIndex index_3 = stm->index(i, stm::Third);

        assert(index_3.data().toInt() != 0);
        QModelIndex index_1 = stm->index(i, stm::First);

        qDebug() << "Value of " <<  index_1.data().toString() << " was changed " << index_3.data().toInt();
      }

  }

  // test copy pv_ to sv_
  stm->CopyPvToSv();
  // test updated data in sv_
  {
    for(int i = 0; i < stm->rowCount(QModelIndex()); ++i){
        QModelIndex index_3 = stm->index(i, stm::Third);
        QModelIndex index_4 = stm->index(i, stm::Fourth);

        assert(index_3.data().toInt() == index_4.data().toInt());
      }
  }

  // change data in sv_
  {
    ChangeValue(stm, 101, "sv");
  }
  // test send sv_ to plc
  clnt.SendSV(stm);
  // test read frpm plc to pv_
  clnt.ReadPV(stm);
  // test updated data in pv_
  {
    for(int i = 0; i < stm->rowCount(QModelIndex()); ++i){
        QModelIndex index_3 = stm->index(i, stm::Third);
        QModelIndex index_4 = stm->index(i, stm::Fourth);

        assert(index_3.data().toInt() == index_4.data().toInt());
      }
  }

  clnt.ClientOff();
  assert(clnt.GetStatus() == UA_STATUSCODE_GOOD);

  qDebug() << "Class ClientOpc test is done";
}

void TestMainWindow(){

}

void TestDataBase(){
  db::DataBase* myBase = new db::DataBase;
  process::Reader rd;

  const QString nameConnection = "TestDataBase";

  int diameter = 0;
  int thickness = 0;
  int width = 0;
  QString note;
  QDate date = QDate::currentDate();
  QTime time = QTime::currentTime();
  QString position;

  int random = qrand(); // Get random double to be inserted into the database
  // connect to data base and verify status
  /*if(myBase->ConnectToDataBase() == db::RESTORE){
      /*stm::StandTableModel* myModel = new stm::StandTableModel(nullptr);
      ChangeValue(myModel, 100, "pv");
      ChangeValue(myModel, 155, "sv");
      // to fill database
      for(int i = 0; i < 2; i++)
        {
          QVariantList data;

          std::ostringstream ostr;

          // insert data to relational table Position values
          if(i == 0){
              for(int j = 0; j < myModel->rowCount(QModelIndex()); ++j){
                  QModelIndex index_1 = myModel->index(j, stm::First);
                  QModelIndex index_3 = myModel->index(j, stm::Third);
                  // insert data to string stream
                  ostr << index_1.data().toString().toStdString() << ": " << index_3.data().toDouble() << ", ";
                }
              std::string ret = ostr.str();
              ret = ret.substr(0, ret.size() - 2);
              position = QString::fromStdString(ret);
            }
          else{
              for(int j = 0; j < myModel->rowCount(QModelIndex()); ++j){
                  QModelIndex index_1 = myModel->index(j, stm::First);
                  QModelIndex index_4 = myModel->index(j, stm::Fourth);
                  // insert data to string stream
                  ostr << index_1.data().toString().toStdString() << ": " << index_4.data().toDouble() << ", ";
                }
              std::string ret = ostr.str();
              ret = ret.substr(0, ret.size() - 2);
              position = QString::fromStdString(ret);

            }


          // Prepare the received random number to be inserted into the database
          diameter = random;
          thickness = random;
          width = random;
          note = "Narmal'no pashla " + QString::number(i);

          data.append(diameter);
          data.append(thickness);
          data.append(width);
          // Prepare message for insertion into the database
          data.append(note);

          data.append(date); // Get the current date to be inserted into the database
          data.append(time); // Get the current time to be inserted into the database

          data.append(position);

          // Insert data into the database
          myBase->InsertIntoFilesTable(data);
        }

      // insert FFX table
      {
        QString file = "/FFX.json";
        std::vector<cmn::RollDiam> tags = std::get<std::vector<cmn::RollDiam>>(rd.Read(file));

        for(const auto& tag : tags){
            QVariantList data;
            data.append(tag.product.diameter);
            data.append(tag.product.diameterAtSQ);
            data.append(tag.product.wallThickness);
            data.append(tag.product.stripsWidth);
            data.append(tag.nameTag);
            data.append(tag.origSV);

            // insert to data base FFX table
            assert( true == myBase->InsertIntoFFXTable(data));
          }
      }
      // insert to FPQM table
      {
        QString file = "/FPSQ.json";
        std::vector<cmn::RollDiam> tags = std::get<std::vector<cmn::RollDiam>>(rd.Read(file));

        for(const auto& tag : tags){
            QVariantList data;
            data.append(tag.product.diameter);
            data.append(tag.nameTag);
            data.append(tag.origDiam);
            data.append(tag.presDiam);
            data.append(tag.origSV);
            data.append(0.0);

            // insert to data base FFX table
            assert( true == myBase->InsertIntoFPSQTable(data));
          }
      }
      // insert to FFXDIAM table
      {
        QString file = "/FFX_diam.json";
        std::vector<cmn::ChangeDiam> tags = std::get<std::vector<cmn::ChangeDiam>>(rd.Read(file));

        for(const auto& tag : tags){
            QVariantList data;
            data.append(tag.typeData);
            data.append(tag.nameTag);
            data.append(tag.origDiam);
            data.append(tag.origDiam);
            data.append(0.0);

            // insert to data base FFX table
            assert(true == myBase->InsertIntoFFXDiamTable(data));
          }
      }
    }*/

  // read from data base table Files
 /* QSqlQuery query;
  QString str = "SELECT * FROM Files;";

  if(!query.exec(str)){
      qDebug() << "Unable to execute query - exiting";
    }

  QSqlRecord rec = query.record();

  // reading from end DB
  while (query.next()){
      qDebug() << "Randon diameter: " << diameter;
      qDebug() << "DB diameter: " << query.value(rec.indexOf("Diameter")).toInt();
      assert(diameter == query.value(rec.indexOf("Diameter tube")).toInt());
      assert(thickness == query.value(rec.indexOf("Thickness")).toInt());
      assert(width == query.value(rec.indexOf("Width")).toInt());
     // assert(note == query.value(rec.indexOf("Note")).toString());
      assert(date == query.value(rec.indexOf("Date")).toDate());
      assert(time == query.value(rec.indexOf("Time")).toTime());
      qDebug() << query.value(rec.indexOf("Position")).toString();
    }*/

  // read from DB FFX table
  /*{
    cmn::Product colDiam;
    colDiam.diameter = 114.3;
    colDiam.wallThickness = 10.0;

    QHash<QString, double> h_table = myBase->GetDataFromDB(FFX, colDiam);
    for(const auto& key : h_table.keys()){
        qDebug() << key << " " << h_table.value(key);
      }

  }*/

  // read from DB FPSQ table
  /*{
    cmn::Product colDiam;
    colDiam.diameter = 114.3;

    QHash<QString, double> h_table = myBase->GetDataFromDB(FPSQ, colDiam);
    for(const auto& key : h_table.keys()){
        qDebug() << key << " " << h_table.value(key);
      }

  }*/

  qDebug() << "Class DataBase test is done";
}

void TestCalculation(){
  calc::Calculation calculation = new calc::Calculation(nullptr);


}

void TestProductSize(){
  // insert data to SAVED table myBase
  db::DataBase* myBase = new db::DataBase;
  {
    ps::ProductSize *ps = new ps::ProductSize;

    ps->SetCurrentProduct("114.3", "10");
    QHash<QString, double> h_table = ps->GetTableSV();

    if(false){
        for(const auto& key : h_table.keys()){
            QVariantList data;
            data.append(key);
            data.append(h_table.value(key));
            assert(true == myBase->InsertIntoSavedTable(data));
          }

      }
    else{
        qDebug() << "Connect to DB, please!";
      }
  }

  // update data to SAVED table myBase
    {
     ps::ProductSize *ps = new ps::ProductSize;
     ps->SetCurrentProduct("114.3", "10");
     QHash<QString, double> h_table = ps->GetTableSV();

     myBase->ConnectToDataBase();

     assert(true == myBase->UpdateDataInDB(SAVED, h_table));

     myBase->DisconnectFromDataBase();

     // upload data from SAVED table DB
     myBase->ConnectToDataBase();

     assert(true == myBase->UploadDataFromDB(SAVED, h_table));

     myBase->DisconnectFromDataBase();
  }

  ps::ProductSize *ps = new ps::ProductSize;
  // get model of diameter tube
  {    
    const QSqlQueryModel* qm = ps->GetModelDiamTube();

    assert("114.3" == qm->record(0).value("Diam_value").toString());
    assert("146.1" == qm->record(1).value("Diam_value").toString());
    assert("159" == qm->record(2).value("Diam_value").toString());
    assert("168.3" == qm->record(3).value("Diam_value").toString());
    assert("177.8" == qm->record(4).value("Diam_value").toString());
    assert("219.1" == qm->record(5).value("Diam_value").toString());
    assert("244.5" == qm->record(6).value("Diam_value").toString());

    assert("114.3" == ps->GetDiamTube());
  }

  // get model thicknesses
  {
    const QSqlQueryModel* qm = ps->GetModelThickness();
    assert("4.7" == qm->record(0).value("Thickness").toString());
    assert("5.5" == qm->record(1).value("Thickness").toString());
    assert("6.35" == qm->record(2).value("Thickness").toString());
    assert("7.7" == qm->record(3).value("Thickness").toString());
    assert("9" == qm->record(4).value("Thickness").toString());
    assert("10" == qm->record(5).value("Thickness").toString());
    assert("12.7" == qm->record(6).value("Thickness").toString());
    assert("14.3" == qm->record(7).value("Thickness").toString());
    assert("15.9" == qm->record(8).value("Thickness").toString());

  }

  // get diameter at SQ
  {
    assert("115.45" == ps->GetDiamAtSQ("114.3"));
  }

  // get width
  {
    assert("346.7" == ps->GetWidth("114.3", "10"));
  }

  // get SV from DB
  {
    QHash<QString, double> h_table = ps->GetTableSV();
    for(const auto& key : h_table.keys()){
        qDebug() << key << " " << h_table.value(key);
      }
  }

  qDebug() << "Class ProductSize test is done";
}

void TestFiles(){

  fls::Files* myFiles = new fls::Files;
  handler::Control* c = new handler::Control;
  stm::StandTableModel* myModel = c->GetStandTableModel();

  double diameter = 999.0;
  double thickness = 555.0;
  double width = 333.0;
  QString note = "Test itself!";
  QDate date = QDate::currentDate();
  QTime time = QTime::currentTime();
  QString position;

  std::ostringstream ostr;
  // read NameTag and PV from stm model
  for(int i = 0; i < myModel->rowCount(QModelIndex()); ++i){
      QModelIndex nameTag = myModel->index(i, stm::First);
      QModelIndex pv = myModel->index(i, stm::Fourth);

      ostr << nameTag.data().toString().toStdString() << ":" <<
              pv.data().toDouble() << ",";
    }
  std::string ret = ostr.str();
  ret = ret.substr(0, ret.size() - 1);
  position = QString::fromStdString(ret);

  QVariantList data;
  //data.append(diameter);
  data.append(diameter);
  data.append(thickness);
  data.append(width);
  data.append(note);
  data.append(date);
  data.append(time);
  data.append(position);

   // save
  {
    QSqlQueryModel* qm = myFiles->GetFilesQueryModel();
    int beforeSaveSize = qm->rowCount();

    qDebug() << beforeSaveSize;
    assert(true == myFiles->SaveReceiptToFilesModel(data));
    qDebug() << qm->rowCount();
    assert((beforeSaveSize + 1) == qm->rowCount());

    for(int nRow = 0; nRow < qm->rowCount(); ++nRow){
        QSqlRecord rec = qm->record(nRow);

        qDebug() << rec.value("id").toInt() << " " <<
                    rec.value("Diameter").toDouble() << " " <<
                    rec.value("Note").toString();
      }
  }
  // remove
  {
    int id = 14;
    assert(true == myFiles->RemoveReceiptFromFilesModel(id));

    QSqlQueryModel* qm = myFiles->GetFilesQueryModel();
    for(int nRow = 0; nRow < qm->rowCount(); ++nRow){
        QSqlRecord rec = qm->record(nRow);

        qDebug() << rec.value("id").toInt() << " " <<
                    rec.value("Diameter").toDouble() << " " <<
                    rec.value("Note").toString() << " " <<
                    rec.value("IsDeleted").toBool();
      }
    //id = -1;
    //assert(false == myFiles->RemoveReceiptFromFilesModel(id));
  }
  // load
  {
    int id = 15;
    //assert()
  }
  // update
  {
    assert(true == myFiles->UpdateFilesModel());
  }
  // restore
  {
    assert(true == myFiles->RestoreReceiptInFilesModel());
  }
  // sort
  {

  }
  // up shift
  {

  }
  // down shift
  {

  }

  qDebug() << "Class Files test is done";
}

void TestModules(){
  //TestConnection();
  //TestPLCData();
  //TestProcess();
  //TestStandTableModel();  
  //TestClientOPC();
  //TestMainWindow();
  //TestDataBase();
  //TestCalculation();
  //TestProductSize();
  //TestFiles();
}



#endif // TESTMODULES_H
