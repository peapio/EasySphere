
#include <iostream>
#include "net/tcp_conn.h"

int main(int argc, char const *argv[])
{
    es::TcpConn conn;
    conn.init("127.0.0.1", 12345);
    conn.connect();
    conn.send("Hello from client");
    LOG_INFO("send message: Hello from client");
    std::string message;
    conn.recv(message);
    LOG_INFO("recv message: %s", message.c_str());
    return 0;
}
