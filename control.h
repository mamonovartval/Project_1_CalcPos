#ifndef CONTROL_H
#define CONTROL_H
#include "clientopc.h"
#include "process.h"
#include "standtablemodel.h"
#include "database.h"
#include "calculation.h"
#include "productsize.h"
#include "files.h"
#include <diameterrolls.h>
#include <QThreadPool>

namespace handler {
  const unsigned int microseconds = 1000;

  class Control : public QObject, public QRunnable
  {
    Q_OBJECT

  public:
    Control();
    void run() override;

    stm::StandTableModel* GetStandTableModel();

    db::DataBase* GetPtrDataBase();
    ps::ProductSize *GetPtrProductSize();
    fls::Files* GetPtrFiles();
    dmt::DiameterRolls* GetPtrDmt();

    ~Control();

  public slots:
    // position value operation - tab Position

    void slotSendSv();
    void slotCalcFromProductSize();


    // data base operation - tab Files
    void slotLoadData(int id);

  signals:
    void sigDebugMode();

  private:
    stm::StandTableModel *ptrStm_;   

    db::DataBase *ptrDb_;
    ps::ProductSize *ptrPs_;
    fls::Files *ptrFls_;
    dmt::DiameterRolls *ptrDmt_;

    client::ClientOPC client_;

    void start();
    void updateSavedSvFromDb();

    bool isSendBusy_ = false;
    bool isDebugMode_ = true;
  };
}


#endif // CONTROL_H
