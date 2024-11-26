#ifndef PLCDATA_H
#define PLCDATA_H
#include <open62541/client_highlevel.h>

#include "common.h"

namespace client {
  namespace detail {
    const int node = 3;
  }
  class PlcData
  {
  protected:
    UA_Int32 ReadData(UA_Client* client, int node, const std::string& var);
    void WriteData(UA_Client* client, int node, const std::string& var, UA_Int32 newValue);
    const UA_StatusCode &IsRead() const;
    const UA_StatusCode &IsWrite() const;

  public:
    PlcData() = default;

  private:
    UA_StatusCode isRead_;
    UA_StatusCode isWrite_;
  };
}


#endif // PLCDATA_H
