#pragma once

#include "common/log.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <memory>

namespace es {

class TcpConn {
public:
    TcpConn();
    ~TcpConn();
    int init(const std::string& ip, int port);
    int connect();
    int listen(int backlog = 5);
    std::unique_ptr<TcpConn> accept();
    int send(const std::string& message);
    int recv(std::string& message);

    int getSockfd() const {
        return m_sockfd;
    };
    std::string getIp() const {
        return m_ip;
    };
    int getPort() const {
        return m_port;
    };
private:
    int setNonBlocking();
    int m_sockfd;
    std::string m_ip;
    int m_port;
};

} // namespace es

