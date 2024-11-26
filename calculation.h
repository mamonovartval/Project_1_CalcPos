#ifndef CALCULATION_H
#define CALCULATION_H

#include <QObject>
#include <common.h>

namespace calc {
  class Calculation : public QObject
  {
    Q_OBJECT

  public:
    Calculation(QObject* pobj = 0);

    void CalculationPosition(const cmn::Product& data, QHash<QString, double>& sv_pos);    
  };
}


#endif // CALCULATION_H
