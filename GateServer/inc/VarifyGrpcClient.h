#ifndef VARIFYGRPCCLIENT_H
#define VARIFYGRPCCLIENT_H

#include <grpcpp/grpcpp.h>

#include "message.grpc.pb.h"
#include "const.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConPool
{
public:
    RPConPool(size_t poolsize, std::string host, std::string port);
    ~RPConPool();

private:
    void Close();
    
    std::atomic<bool> b_stop_;
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
    std::condition_variable cond_;
    std::mutex mutex_;
};

class VarifyGrpcClient : public Singleton<VarifyGrpcClient>
{
    friend class Singleton<VarifyGrpcClient>;

public:
    GetVarifyRsp GetVarifyCode(std::string email);

    ~VarifyGrpcClient() = default;

private:
    VarifyGrpcClient();
    std::unique_ptr<VarifyService::Stub> stub_;
};

#endif // VARIFYGRPCCLIENT_H