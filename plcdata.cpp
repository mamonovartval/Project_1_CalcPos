#include "plcdata.h"

namespace client {
  /*const UA_StatusCode &PlcData::GetStatus() const
  {
    return statusReading_;
  }

  size_t HasherVar::operator()(const std::string &name) const noexcept
  {
    size_t h_str = ptr_hasher_(name);

    return h_str;
  }*/

  UA_Int32 PlcData::ReadData(UA_Client *client, int node, const std::string& var)
  {
    UA_Int32 value;
    UA_Variant* val = UA_Variant_new();
    // const cast string to char
    char* ch_var = const_cast<char*>(var.c_str());

    isRead_ = UA_Client_readValueAttribute(client, UA_NODEID_STRING(node, ch_var), val);

    if(isRead_ == UA_STATUSCODE_GOOD && val->type == &UA_TYPES[UA_TYPES_INT32]) {
            value = *(UA_Int32*)val->data;
    }
    UA_Variant_delete(val);

    return value;
  }

  void PlcData::WriteData(UA_Client *client, int node, const std::string& var, UA_Int32 newValue)
  {
    UA_Variant* val = UA_Variant_new();
    UA_Variant_setScalarCopy(val, &newValue, &UA_TYPES[UA_TYPES_INT32]);
    // const cast string to char
    char* ch_var = const_cast<char*>(var.c_str());

    isWrite_ = UA_Client_writeValueAttribute(client, UA_NODEID_STRING(node, ch_var), val);

    //if(isWrite_ == UA_STATUSCODE_GOOD) {
    //        printf("the value is: %li\n", value);
    //}
    UA_Variant_delete(val);
  }

  const UA_StatusCode &PlcData::IsRead() const
  {
    return isRead_;
  }

  const UA_StatusCode &PlcData::IsWrite() const
  {
    return isWrite_;
  }
}

