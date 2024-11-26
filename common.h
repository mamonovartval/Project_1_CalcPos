#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <string>
#include <vector>
#include <QtCore>

namespace cmn {

  struct Coordinate{

    QString nameAxis;
    int value = 0;

    bool operator==(const Coordinate& other) const{
      return nameAxis == other.nameAxis && value == other.value;
    }
  };

  struct BasePosAndLimits
  {
    double base = 0.0;
    double min = 0.0;
    double max = 0.0;
  };

  struct Device{
    QString nameTag;
    QString nameStand;
    QString nameAxis;
    BasePosAndLimits posLim;
  };

  struct SettingOPC{
    QString ip_adress;
    QString port_adress;
  };  

  struct Product{
    double diameter = 0.0;
    double diameterAtSQ = 0.0;
    double wallThickness = 0.0;
    double stripsWidth = 0.0;    
  };

  struct RollDiam{
    Product product;
    QString nameTag;
    double presDiam = 0.0;
    double origDiam = 0.0;
    double origSV = 0.0;
    double calcSV = 0.0;
  };

  struct ChangeDiam{
    QString typeData;
    QString nameTag;
    double origDiam;
    std::vector<double> arr_origDiam;
  };

  struct Variable
  {
    Variable(const QString db_name, const QString tag_name){
      variable_ = "\"" + db_name.toStdString() + "\"" + "." + "\"" + tag_name.toStdString() + "\"";
    }

    const std::string& GetVariable() const{
      return variable_;
    }

  private:
    std::string variable_;
  };
}


#endif // STRUCTURES_H
