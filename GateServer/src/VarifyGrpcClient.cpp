#include "VarifyGrpcClient.h"

VarifyGrpcClient::VarifyGrpcClient()
{
    std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
    stub_ = VarifyService::NewStub(channel);
}

GetVarifyRsp VarifyGrpcClient::GetVarifyCode(std::string email)
{
    ClientContext context;
    GetVarifyReq request;
    GetVarifyRsp reply;

    request.set_email(email);

    Status status = stub_->GetVarifyCode(&context, request, &reply);
    if (status.ok())
    {
        return reply;
    }
    else
    {
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}

RPConPool::RPConPool(size_t poolsize, std::string host, std::string port)
    : poolSize_(poolsize), host_(host), port_(port), b_stop_(false)
{
    std::shared_ptr<Channel> channel = grpc::CreateChannel(host_ + ":" + port_, grpc::InsecureChannelCredentials());
    connections_.push(VarifyService::NewStub(channel));
}

RPConPool::~RPConPool()
{
    std::lock_guard<std::mutex> lock(mutex_);
    Close();
    while(!connections_.empty())
    {
        connections_.pop();
    }
}

void RPConPool::Close()
{
    b_stop_ = true;
    cond_.notify_all();
}
