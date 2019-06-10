#include "ws/ServerRequestAcceptor.hpp"
#include "ws/ServerCallback.hpp"
#include "ws/Session.hpp"
#include "ws/ServerListener.hpp"

using namespace ws;
using namespace net;
using exceptions::Exception;

ServerRequestAcceptor::ServerRequestAcceptor(const int port, ws::ServerListener &listener):
ServerCallback(listener), ioCtx(1), endPoint(EndPoint(tcp::v4(), port)), acceptor(Acceptor(ioCtx, endPoint)){
    this->acceptor.non_blocking(true);
}

ServerRequestAcceptor::~ServerRequestAcceptor(){}

Session *ServerRequestAcceptor::accept(){

    try{
        TCPSocket socket(this->ioCtx);
        this->acceptor.accept(socket);

        FlatBuffer buffer;
        HTTPRequest request;
        http::read(socket, buffer, request);

        if(!this->isAcceptable(request)) return NULL;

        ServerRequestAcceptor *me = this;
        WSocket ws(std::move(socket));

        ws.set_option(
            websocket::stream_base::timeout::suggested(boost::beast::role_type::server)
        );

        ws.set_option(ws::websocket::stream_base::decorator([&me](HTTPWSResponse& m){
            me->onAccept(m);
        }));
        ws.accept(request);

        return new Session(std::move(ws), *this);
    }
    catch(const std::exception& e){}

    return NULL;
}

void ServerRequestAcceptor::close(){
    this->acceptor.close();
}

bool ServerRequestAcceptor::isAcceptable(HTTPRequest &request) const {
    if(!websocket::is_upgrade(request)) return false;
    return this->listener.onIsAcceptable(request);
}

void ServerRequestAcceptor::onAccept(HTTPWSResponse &response) const {
    this->listener.onAccept(response);
}
