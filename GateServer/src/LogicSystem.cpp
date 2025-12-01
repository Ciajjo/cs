#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VarifyGrpcClient.h"

LogicSystem::~LogicSystem()
{
}

LogicSystem::LogicSystem()
{
    RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection)
           {
        beast::ostream(connection->_response.body()) << "receive get_test req";
        int i =0;
        for (auto &elem : connection->_get_params)
        {
            i++;
            beast::ostream(connection->_response.body()) << "parm" << i << "key is " << elem.first;
            beast::ostream(connection->_response.body()) << " , " << "parm" << i << "value is " << elem.second << std::endl;
        } });

    RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection)
            { 
        auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
        std::cout << "receive body is " << body_str << std::endl; 
        connection->_response.set(http::field::content_type, "text/json"); 

        json root;
        if (!json::accept(body_str)) {
            std::cout << "Failed to parse JSON data!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string jsonstr = root.dump();
            beast::ostream(connection->_response.body()) << jsonstr;
            return false; // 或返回错误标志
        }

        json src_root = json::parse(body_str);
        if(!src_root.contains("email"))
        {
            std::cout << "Failed to find JSON key!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string jsonstr = root.dump();
            beast::ostream(connection->_response.body()) << jsonstr;
            return false; // 或返回错误标志
        }

        auto email = src_root["email"].get<std::string>();
        GetVarifyRsp rsp = VarifyGrpcClient::GetInstance()->GetVarifyCode(email);
        std::cout << "email is " << email << std::endl;
        root["error"] = rsp.error();
        root["email"] = src_root["email"].get<std::string>();
        std::string jsonstr = root.dump();
        beast::ostream(connection->_response.body()) << jsonstr;
        return true; });
}

void LogicSystem::RegGet(std::string url, HttpHandler handler)
{
    _get_handlers.insert(std::make_pair(url, handler));
}

void LogicSystem::RegPost(std::string url, HttpHandler handler)
{
    _post_handlers.insert(std::make_pair(url, handler));
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con)
{
    if (_get_handlers.find(path) == _get_handlers.end())
    {
        return false;
    }

    _get_handlers[path](con);
    return true;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con)
{
    if (_post_handlers.find(path) == _post_handlers.end())
    {
        return false;
    }

    _post_handlers[path](con);
    return true;
}