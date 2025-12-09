#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOServicePool.h"

CServer::CServer(boost::asio::io_context &ioc, unsigned short port)
    : _acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _ioc(ioc) {}

void CServer::start()
{
    auto self = shared_from_this();
    auto &io_context = AsioIOServicePool::GetInstance()->GetIOService();
    std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
    _acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec)
                           {
                                            try
                                            {
                                                // 出错放弃链接，监听其他链接
                                                if (ec)
                                                {
                                                    self->start();
                                                    return;
                                                }
                                                //创建链接，有HttpConnection管理
                                                new_con->start();
                                                //继续坚挺
                                                self->start();
                                            }
                                            catch(const std::exception& e)
                                            {} });
}
