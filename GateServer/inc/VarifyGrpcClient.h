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