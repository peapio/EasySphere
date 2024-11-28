#include "tcp_conn.h"

namespace es {

TcpConn::TcpConn() : m_sockfd(-1) {}

TcpConn::~TcpConn()
{
    close(m_sockfd);
}

int TcpConn::init(const std::string& ip, int port) {
    m_ip = ip;
    m_port = port;
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd == -1) {
        LOG_ERROR("socket create failed");
        return -1;
    }
    return 0;
}

int TcpConn::connect() {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(m_port);
    server_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());

    if (::connect(m_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        LOG_ERROR("connect failed, %d", errno);
        return -1;
    }
    return 0;
}

int TcpConn::listen(int backlog) {

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(m_port);
    server_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());

    int opt = 1;
    if(setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        LOG_ERROR("setsockopt failed, %d", errno);
        return -1;
    }

    if (::bind(m_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        LOG_ERROR("bind failed, %d", errno);
        return -1;
    }
    LOG_INFO("listen on %s:%d", m_ip.c_str(), m_port);
    if (::listen(m_sockfd, backlog) < 0) {
        LOG_ERROR("listen failed, %d", errno);
        return -1;
    }
    return 0;
}

std::unique_ptr<TcpConn> TcpConn::accept() {

    int new_socket;
    struct sockaddr_in new_addr;
    int addrlen = sizeof(new_addr);

    if ((new_socket = ::accept(m_sockfd, (struct sockaddr *)&new_addr, (socklen_t*)&addrlen)) < 0) {
        LOG_ERROR("accept failed, %d", errno);
        return nullptr;
    }
    LOG_INFO("new client connected, ip: %s, port: %d", inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));
    std::unique_ptr<TcpConn> new_conn = std::make_unique<TcpConn>();
    new_conn->m_sockfd = new_socket;
    new_conn->m_ip = inet_ntoa(new_addr.sin_addr);
    new_conn->m_port = ntohs(new_addr.sin_port);
    if(new_conn->setNonBlocking() < 0) {
        LOG_ERROR("set non blocking failed");
        return nullptr;
    }
    return new_conn;
}

int TcpConn::setNonBlocking() {
    int flags = ::fcntl(m_sockfd, F_GETFL, 0);
    if (flags == -1) {
        LOG_ERROR("fcntl failed, %d", errno);
        return -1;
    }
    flags |= O_NONBLOCK;
    return ::fcntl(m_sockfd, F_SETFL, flags);
}

int TcpConn::send(const std::string& message) {
    return ::write(m_sockfd, message.c_str(), message.size());
}

int TcpConn::recv(std::string& message) {
    const int buffer_size = 1024;
    char buffer[buffer_size] = {0};
    int valread = 0;
    while (true) {
        int ret = ::read(m_sockfd, buffer, buffer_size);
        if (ret == 0) {
            break;
        }
        if (ret < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            else {
                LOG_ERROR("recv failed, %d", errno);
                break;
            }
        }
        message.append(buffer, ret);
        valread += ret;
    }
    return valread;
}

} // namespace es