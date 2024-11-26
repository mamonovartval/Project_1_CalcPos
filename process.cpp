#include "process.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>

process::Responce process::Reader::Read(const QString &in)
{
  QJsonDocument doc = readJSON(in);

  // verifying doc
  if(doc.isEmpty()){
      qWarning() << "There is no any data in json file: " << in;
      return {};
    }
  if(doc.object().keys().first() == detail::setting){
      return readSetting(doc.object());
    }
  else if(doc.object().keys().first() == detail::HOME){
      return readTagsToStandModel(doc.object(), detail::HOME);
    }
  else if(doc.object().keys().first() == detail::FPSQ){
      return readTagsToFPSQModel(doc.object(), detail::FPSQ);
    }
  else if(doc.object().keys().first() == detail::FFXD){
      return readTagsToFFXDModel(doc.object(), detail::FFXD);
    }
  else if(doc.object().keys().first() == detail::FFX){
      return readTagsToFFXModel(doc.object(), detail::FFX);
    }
  else if(doc.object().keys().first() == detail::DIAMLIST){
      return readDiameter(doc.object(), detail::DIAMLIST);
    }
  else if(doc.object().keys().first() == detail::THICK){
      return readDiameter(doc.object(), detail::THICK);
    }
  else{
      return {};
    }
}



QHash<QString, double> process::Reader::readStringToSV(const QString &str)
{
  QHash<QString, double> result;
  QStringList pieces = str.split(",");
  foreach (QString piece, pieces) {
      QStringList p = piece.split(":");
      double value = p[1].toDouble();
      result[p[0]] = value;
    }
  return result;
}

cmn::Device process::Reader::separateTag(std::string_view str)
{
  cmn::Device result;
  //looking for first position with underscore
  size_t pos = str.find_first_of('_');
  //to std::string
  std::string nameTag = (std::string)str;
  std::string nameStand = (std::string)str.substr(0, pos);
  std::string nameAxis = (std::string)str.substr(pos + 1, str.size() - 1);
  std::string newNameAxis = changeUnderscore(nameAxis, '_', ' ');
  // to QString
  result.nameTag = QString::fromUtf8(nameTag.c_str());
  result.nameStand = QString::fromUtf8(nameStand.c_str());
  result.nameAxis = QString::fromUtf8(newNameAxis.c_str());

  return result;
}

std::string process::Reader::changeUnderscore(const std::string &str, const char sign, const char newSign)
{
  std::string result;
  for(size_t i = 0; i < str.size(); ++i){
      if(str[i] == sign){
          result += newSign;
        }
      else{
          result += str[i];
        }

    }
  return result;
}

cmn::SettingOPC process::Reader::readSetting(const QJsonObject &obj)
{
  cmn::SettingOPC result;
  QJsonValue sett = obj.value(detail::setting);
  result.ip_adress = sett[detail::ip_adr].toString();
  result.port_adress = sett[detail::port].toString();

  return result;
}

std::vector<cmn::Device> process::Reader::readTagsToStandModel(const QJsonObject &obj, const QString &nameDB)
{
  std::vector<cmn::Device> result;
  // getting value for test db
  QJsonValue db = obj.value(nameDB);
  // verifying value is array
  if(db.isObject()){
      // take all keys which equal name tags
      for(const auto& key : db.toObject().keys()){
         cmn::Device deviceCopy = separateTag(key.toStdString());

         if(db.toObject().value(key).isObject()){
             deviceCopy.posLim.base = db.toObject().value(key).toObject().value("base_position").toDouble();
             deviceCopy.posLim.min = db.toObject().value(key).toObject().value("min").toDouble();
             deviceCopy.posLim.max = db.toObject().value(key).toObject().value("max").toDouble();
           }
          result.push_back(deviceCopy);
        }
    }
  else{
      qDebug() << "Stand model: Result is empty!";
    }
  return result;
}

std::vector<cmn::RollDiam> process::Reader::readTagsToFPSQModel(const QJsonObject &obj, const QString &nameDB)
{
  std::vector<cmn::RollDiam> result;
  // getting value for test db
  QJsonValue db = obj.value(nameDB);
  // verifying value is array
  if(db.isObject()){
      // take all keys which equal name tags

      for(const auto& diamTube : db.toObject().keys()){
          cmn::RollDiam rollDiamCopy;
          rollDiamCopy.product.diameter = std::stod(diamTube.toStdString());

          QJsonValue tags = db.toObject().value(diamTube);

          for(const auto& tagKey : tags.toObject().keys()){

              rollDiamCopy.nameTag = tagKey;
              if(tags.toObject().value(tagKey).toObject().value("PD").isDouble()){
                  rollDiamCopy.presDiam = tags.toObject().value(tagKey).toObject().value("PD").toDouble();
                }
              if(tags.toObject().value(tagKey).toObject().value("OrD").isDouble()){
                  rollDiamCopy.origDiam = tags.toObject().value(tagKey).toObject().value("OrD").toDouble();
                }
              rollDiamCopy.origSV = tags.toObject().value(tagKey).toObject().value("OrSV").toDouble();
              rollDiamCopy.calcSV = tags.toObject().value(tagKey).toObject().value("CalcSV").toDouble();

              result.push_back(rollDiamCopy);
            }          
        }
    }
  else{
      qDebug() << "FPSQ model: Result is empty!";
    }
  return result;
}

std::vector<cmn::ChangeDiam> process::Reader::readTagsToFFXDModel(const QJsonObject &obj, const QString &nameDB)
{
  std::vector<cmn::ChangeDiam> result;
  // getting value for test db
  QJsonValue db = obj.value(nameDB);
  // verifying value is array
  if(db.isObject()){
      // take all keys which equal name tags

      for(const auto& typeData : db.toObject().keys()){
          QJsonValue tags = db.toObject().value(typeData);
          for(const auto& tagKey : tags.toObject().keys()){
              cmn::ChangeDiam changeDiamCopy;
              changeDiamCopy.typeData = typeData;

              changeDiamCopy.nameTag = tagKey;
              if(tags.toObject().value(tagKey).isArray()){
                  bool flag = true;
                  for(const auto& val : tags.toObject().value(tagKey).toArray()){
                      changeDiamCopy.arr_origDiam.push_back(val.toDouble());
                      if(flag){
                          changeDiamCopy.origDiam = val.toDouble();
                          flag = false;
                        }
                    }
                }
              result.push_back(changeDiamCopy);
            }
        }
    }
  else{
      qDebug() << "FFXD model: Result is empty!";
    }
  return result;
}

std::vector<cmn::RollDiam> process::Reader::readTagsToFFXModel(const QJsonObject &obj, const QString &nameDB)
{
  std::vector<cmn::RollDiam> result;
  // getting value for test db
  QJsonValue db = obj.value(nameDB);
  // verifying value is array
  if(db.isObject()){
      // take all keys which equal name tags

      for(const auto& diamTube : db.toObject().keys()){
          cmn::RollDiam rollDiamCopy;
          rollDiamCopy.product.diameter = std::stod(diamTube.toStdString());

          QJsonValue keys = db.toObject().value(diamTube);

          bool flag = true;
          for(const auto& key : keys.toObject().keys()){
              // first key equal diameterAtSQ
              if(flag){
                  rollDiamCopy.product.diameterAtSQ = keys.toObject().value(key).toDouble();
                  flag = false;
                  continue;
                }

              QJsonValue thickness = keys.toObject().value(key);
              rollDiamCopy.product.wallThickness = thickness.toObject().value(detail::THICK).toDouble();
              rollDiamCopy.product.stripsWidth = thickness.toObject().value(detail::WIDTH).toDouble();

              for(const auto& tag : thickness.toObject().keys()){

                  if(tag == detail::THICK || tag == detail::WIDTH){
                      continue;
                    }

                  rollDiamCopy.nameTag = tag;
                  rollDiamCopy.origSV = thickness.toObject().value(tag).toObject().value("S.V.").toDouble();
                  result.push_back(rollDiamCopy);
                }
            }
        }
    }
  else{
      qDebug() << "FFX model: Result is empty!";
    }
  return result;
}

QStringList process::Reader::readDiameter(const QJsonObject &obj, const QString &nameDB)
{
  QStringList result;
  if(obj.value(nameDB).isArray()){
      for(const auto a : obj.value(nameDB).toArray()){
          result << QString::number(a.toDouble());
        }
    }
  else{
      qDebug() << "File " << nameDB << " is empty!";
    }
  return result;
}

QStringList process::Reader::readThickness(const QJsonObject &obj, const QString &nameDB)
{
  QStringList result;
  if(obj.value(nameDB).isArray()){
      for(const auto a : obj.value(nameDB).toArray()){
          result << QString::number(a.toDouble());
        }
    }
  else{
      qDebug() << "File " << nameDB << " is empty!";
    }
  return result;
}

QJsonDocument process::Reader::readJSON(const QString &in)
{
  QString path = QCoreApplication::applicationDirPath() + in;
  QString val;
  QFile file(path);
  QJsonDocument doc;

  if(!file.exists()){
      qWarning() << "File does not exists";
      return doc;
    }

  QString errMsg;
  QFileDevice::FileError err = QFileDevice::NoError;
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      errMsg = file.errorString();
      err = file.error();
  }
  val = file.readAll();
  file.close();
  QJsonParseError jsonError;
  doc = QJsonDocument::fromJson(val.toUtf8(), &jsonError);
  if (jsonError.error != QJsonParseError::NoError){
          qDebug() << jsonError.errorString() << ' ' << jsonError.offset << " char into input string";
      }

  return doc;
}

