#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context &ioc, unsigned short port)
    : _acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _ioc(ioc), _socket(ioc) {}

void CServer::start()
{
    auto self = shared_from_this();
    _acceptor.async_accept(_socket, [self](beast::error_code ec)
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
                                                std::make_shared<HttpConnection>(std::move(self->_socket))->start();
                                                //继续坚挺
                                                self->start();
                                            }
                                            catch(const std::exception& e)
                                            {} });
}
