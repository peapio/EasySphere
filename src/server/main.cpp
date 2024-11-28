#include <iostream>
#include "common/log.h"
#include "net/tcp_conn.h"
#include <dbg.h>

int main() {
    LOG_INFO("This is easysphere server");
    es::TcpConn conn;
    conn.init("127.0.0.1", 12345);
    conn.listen();
    std::unique_ptr<es::TcpConn> new_conn = conn.accept();
    std::string message;
    new_conn->recv(message);
    LOG_INFO("recv message: %s", message.c_str());
    new_conn->send("Hello from server");

    return 0;
}
