#include "publisher.h"

#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

Publisher::Publisher(const char* topic, const char* ip, int port) :
    topic(topic),
    topic_len(strlen(topic) + 1),
    addr(),
    sock(-1)
{
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    create_socket();
}

void Publisher::create_socket()
{
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    int trueflag = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &trueflag, sizeof(trueflag));
}

bool Publisher::send_string(const char* data)
{
    return send_void_ptr(data, strlen(data));
}

bool Publisher::send_empty()
{
    return send_void_ptr(nullptr, 0);
}

bool Publisher::send_bytes(const uint8_t* data, int len)
{
    return send_void_ptr(data, len);
}

bool Publisher::send_void_ptr(const void* data, int len)
{
    int packet_len = topic_len + len;
    if (packet_len > UDP_MAX_LEN) {
        return false;
    }
    uint8_t buffer[UDP_MAX_LEN];
    memcpy(buffer, topic, topic_len);
    memcpy(&buffer[topic_len], data, len);
    int bytes_sent = sendto(sock,
                            buffer,
                            packet_len,
                            0,
                            reinterpret_cast<sockaddr*>(&addr),
                            sizeof(addr));
    return bytes_sent == packet_len;
}
