//
// Created by lewis on 2019/8/28.
//

#ifndef ENCODE_H264_DEMO_UDP_SENDER_H
#define ENCODE_H264_DEMO_UDP_SENDER_H

#include <arpa/inet.h>
#include <sys/types.h>

namespace udpsender {
    class Udp_Sender {
    public:
        void InitUdpSocket( uint16_t local_port,
                            const char* remote_address,
                            uint16_t remote_port );
        void SendData(uint8_t* data, int data_len, int mtu);
    private:
        struct sockaddr_in local_address_;
        struct sockaddr_in remote_address_;
        int socket_fd_;
    };
}
#endif //ENCODE_H264_DEMO_UDP_SENDER_H
