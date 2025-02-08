#include "subscriber.h"

#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

Subscriber::Subscriber(const char* topic, const char* ip, int port) :
    topic(topic),
    addr(),
    sock(-1),
    recv_buffer(),
    recv_len()
{
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    create_socket();
}

void Subscriber::create_socket()
{
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    int trueflag = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &trueflag, sizeof(trueflag));
    bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
}

bool Subscriber::recv()
{
    recv_len = ::recv(sock, recv_buffer, UDP_MAX_LEN, MSG_DONTWAIT);
    if ((recv_len > 0) && topic_matches(get_recv_topic())) {
        recv_buffer[recv_len] = 0;
        return true;
    }
    return false;
}

const char* Subscriber::get_recv_topic() const
{
    if (recv_len) {
        return (const char*)recv_buffer;
    }
    return nullptr;
}

const char* Subscriber::get_string() const
{
    if (recv_len) {
        return (const char*)get_void_ptr();
    }
    return nullptr;
}

const uint8_t* Subscriber::get_bytes() const
{
    if (recv_len) {
        return (const uint8_t*)get_void_ptr();
    }
    return nullptr;
}

const void* Subscriber::get_void_ptr() const
{
    if (recv_len > get_data_offset()) {
        return &recv_buffer[get_data_offset()];
    }
    return nullptr;
}

int Subscriber::get_len() const
{
    return recv_len - get_data_offset();
}

bool Subscriber::topic_matches(const char* recv_topic) const
{
    for (int i = 0; i < get_data_offset(); i++) {
        if ((topic[i] == '*') || ((recv_topic[i] == '\0') && (topic[i] == '\0'))) {
            return true;
        } else if ((recv_topic[i] != topic[i]) || ((recv_topic[i] == '\0') || (topic[i] == '\0'))) {
            return false;
        }
    }
    return false;
}

int Subscriber::get_data_offset() const
{
    return strlen(get_recv_topic()) + 1;
}
