#include <QTest>
#include "connection.h"

class Test_Connection : public QObject{
  Q_OBJECT
private slots:
  void Connect();
  void Disconnect();
  /*void SetSetting(SettingOPC);
  const SettingOPC& GetSetting() const;
  const UA_StatusCode& GetStatus() const;
  const UA_Client* GetClient() const;
  UA_Client* GetClient();*/
};

void Test_Connection::Connect()
{
    Connection myConnect;
    std::string ip = "10.8.200.51";
    std::string port = "4840";
    SettingOPC setup{ip, port};
    myConnect.SetSetting(setup);
    QCOMPARE("10.8.200.51", myConnect.GetSetting().ip_adress);
    QCOMPARE("4840", myConnect.GetSetting().port_adress);

    myConnect.Connect();
    QCOMPARE(myConnect.GetStatus(), UA_STATUSCODE_GOOD);
}

void Test_Connection::Disconnect()
{
  Connection myConnect;
  std::string ip = "10.8.200.51";
  std::string port = "4840";
  SettingOPC setup{ip, port};
  myConnect.SetSetting(setup);
  QCOMPARE("10.8.200.51", myConnect.GetSetting().ip_adress);
  QCOMPARE("4840", myConnect.GetSetting().port_adress);


}

QTEST_MAIN(Test_Connection)
#include "testmodules.moc"
