#pragma once
#include <string_view>
#include <variant>

#include "common.h"

/*
There is parsing all incoming and outcoming files.
*/

namespace process {

  namespace detail{
      const QString setting = "SettingsOPC";
      const QString ip_adr = "ip_adress";
      const QString port = "port_adress";
      const QString db_test = "Test_for_FFX";
      const QString db_pv = "PC_PV";
      const QString db_sv = "PC_SV";
      const QString db_com = "PC_COM";
      const QString db_pipe = "PC_PIPE";
      const QString db_product = "PC_PRODUCT_INFO";
      const QString db_diameter = "Roll Diameter";
      const QString HOME = "Home";
      const QString FPSQ = "FPSQ";
      const QString FFXD = "FFX_diam";
      const QString FFX = "FFX";
      const QString THICK = "thickness";
      const QString WIDTH = "width";
      const QString DIAMLIST = "Diameter";
  }

  using Responce = std::variant<std::monostate, cmn::SettingOPC, std::vector<cmn::Device>,
  std::vector<cmn::RollDiam>, std::vector<cmn::ChangeDiam>, QStringList, QHash<QString, double>>;

  class Reader{
  public:
    Reader() = default;

    Responce Read(const QString& in);



    QHash<QString, double> readStringToSV(const QString& str);

  private:
    cmn::Device separateTag(std::string_view str);
    std::string changeUnderscore(const std::string& str, const char sign,
                                 const char newSign);
    std::vector<cmn::Device> readTagsToStandModel(const QJsonObject &obj, const QString &nameDB);
    std::vector<cmn::RollDiam> readTagsToFPSQModel(const QJsonObject &obj, const QString &nameDB);
    std::vector<cmn::ChangeDiam> readTagsToFFXDModel(const QJsonObject &obj, const QString &nameDB);
    std::vector<cmn::RollDiam> readTagsToFFXModel(const QJsonObject &obj, const QString &nameDB);

    QStringList readDiameter(const QJsonObject &obj, const QString &nameDB);
    QStringList readThickness(const QJsonObject &obj, const QString &nameDB);
    cmn::SettingOPC readSetting(const QJsonObject& obj);
    QJsonDocument readJSON(const QString& in);
    //QHash<QString, double> readStringToSV(const QString& str);
  };
} // end namespace


