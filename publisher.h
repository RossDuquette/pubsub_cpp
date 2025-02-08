#ifndef PUBSUB_CPP_PUBLISHER_H_
#define PUBSUB_CPP_PUBLISHER_H_

#include "conf.h"

#include <netinet/in.h>

class Publisher {
    public:
        Publisher(const char* topic, const char* ip=UDP_DEFAULT_IP, int port=UDP_DEFAULT_PORT);

        bool send_string(const char* data);
        bool send_empty();
        bool send_bytes(const uint8_t* data, int len);
        bool send_void_ptr(const void* data, int len);

    private:
        void create_socket();

        const char* topic;
        int topic_len;
        sockaddr_in addr;
        int sock;
};

#endif
