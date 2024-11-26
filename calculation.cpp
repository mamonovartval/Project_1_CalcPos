#include "calculation.h"

namespace calc {

  Calculation::Calculation(QObject *pobj) : QObject(pobj)
  {

  }

  void Calculation::CalculationPosition(const cmn::Product &data, QHash<QString, double> &sv_pos)
  {
    return;
  }

}
