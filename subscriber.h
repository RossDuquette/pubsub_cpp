#ifndef PUBSUB_CPP_SUBSCRIBER_H_
#define PUBSUB_CPP_SUBSCRIBER_H_

#include "conf.h"

#include <cstdint>
#include <netinet/in.h>

class Subscriber {
    public:
        Subscriber(const char* topic, const char* ip=UDP_DEFAULT_IP, int port=UDP_DEFAULT_PORT);

        bool recv();
        const char* get_recv_topic() const;
        const char* get_string() const;
        const uint8_t* get_bytes() const;
        const void* get_void_ptr() const;
        int get_len() const;

    private:
        void create_socket();
        bool topic_matches(const char* recv_topic) const;
        int get_data_offset() const;

        const char* topic;
        sockaddr_in addr;
        int sock;
        uint8_t recv_buffer[UDP_MAX_LEN+1];
        int recv_len;
};

#endif
