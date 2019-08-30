//
// Created by lewis on 2019/8/28.
//

#include "udp_sender.h"
#include <iostream>
#include <zconf.h>
#include <cstring>

void udpsender::Udp_Sender::InitUdpSocket(  uint16_t local_port,
                                            const char* remote_address,
                                            uint16_t remote_port) {
    socket_fd_ = socket(AF_INET,SOCK_DGRAM,0);
    local_address_.sin_family = AF_INET;
    local_address_.sin_port = htons(local_port);
    local_address_.sin_addr.s_addr = htonl(INADDR_ANY);
    remote_address_.sin_family = AF_INET;
    remote_address_.sin_addr.s_addr = inet_addr(remote_address);
    remote_address_.sin_port = htons(remote_port);
    if(bind(socket_fd_,(struct sockaddr *)&local_address_, sizeof(struct sockaddr)) == -1) {
        std::cout<<"band failed!!!"<<std::endl;
        exit(1);
    }
}

void udpsender::Udp_Sender::SendData(uint8_t* data, int data_len, int mtu) {
    if (mtu == 0) {
        sendto(socket_fd_, data, data_len, 0, (struct sockaddr*)&remote_address_, sizeof(struct sockaddr_in));
        usleep(1000);
    } else if (data_len < mtu) {
        sendto(socket_fd_, data, data_len, 0, (struct sockaddr*)&remote_address_, sizeof(struct sockaddr_in));
        usleep(1000);
    } else {
        int udp_packet_count = (data_len / mtu) + 1;
        for (int i = 0; i<udp_packet_count; i++) {
            if (i == (udp_packet_count - 1)) {
                sendto(socket_fd_, data+i*mtu, data_len % mtu, 0, (struct sockaddr*)&remote_address_, sizeof(struct sockaddr_in));
                usleep(1000);
            } else {
                sendto(socket_fd_, data+i*mtu, mtu, 0, (struct sockaddr*)&remote_address_, sizeof(struct sockaddr_in));
                usleep(1000);
            }
        }
    }

}