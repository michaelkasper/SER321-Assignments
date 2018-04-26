/**
 * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!
 */

#ifndef JSONRPC_CPP_STUB_WAYPOINTSERVERSTUB_H_
#define JSONRPC_CPP_STUB_WAYPOINTSERVERSTUB_H_

#include <jsonrpccpp/server.h>

class waypointserverstub : public jsonrpc::AbstractServer<waypointserverstub>
{
    public:
        waypointserverstub(jsonrpc::AbstractServerConnector &conn, jsonrpc::serverVersion_t type = jsonrpc::JSONRPC_SERVER_V2) : jsonrpc::AbstractServer<waypointserverstub>(conn, type)
        {
            this->bindAndAddMethod(jsonrpc::Procedure("get", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_OBJECT, "param01",jsonrpc::JSON_STRING, NULL), &waypointserverstub::getI);
            this->bindAndAddMethod(jsonrpc::Procedure("getNames", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_ARRAY,  NULL), &waypointserverstub::getNamesI);
            this->bindAndAddMethod(jsonrpc::Procedure("remove", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param01",jsonrpc::JSON_STRING, NULL), &waypointserverstub::removeI);
            this->bindAndAddMethod(jsonrpc::Procedure("add", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param01",jsonrpc::JSON_OBJECT, NULL), &waypointserverstub::addI);
            this->bindAndAddMethod(jsonrpc::Procedure("saveToJsonFile", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN,  NULL), &waypointserverstub::saveToJsonFileI);
            this->bindAndAddMethod(jsonrpc::Procedure("resetFromJsonFile", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN,  NULL), &waypointserverstub::resetFromJsonFileI);
        }

        inline virtual void getI(const Json::Value &request, Json::Value &response)
        {
            response = this->get(request[0u].asString());
        }
        inline virtual void getNamesI(const Json::Value &request, Json::Value &response)
        {
            (void)request;
            response = this->getNames();
        }
        inline virtual void removeI(const Json::Value &request, Json::Value &response)
        {
            response = this->remove(request[0u].asString());
        }
        inline virtual void addI(const Json::Value &request, Json::Value &response)
        {
            response = this->add(request[0u]);
        }
        inline virtual void saveToJsonFileI(const Json::Value &request, Json::Value &response)
        {
            (void)request;
            response = this->saveToJsonFile();
        }
        inline virtual void resetFromJsonFileI(const Json::Value &request, Json::Value &response)
        {
            (void)request;
            response = this->resetFromJsonFile();
        }
        virtual Json::Value get(const std::string& param01) = 0;
        virtual Json::Value getNames() = 0;
        virtual bool remove(const std::string& param01) = 0;
        virtual bool add(const Json::Value& param01) = 0;
        virtual bool saveToJsonFile() = 0;
        virtual bool resetFromJsonFile() = 0;
};

#endif //JSONRPC_CPP_STUB_WAYPOINTSERVERSTUB_H_
