//
// Created by server on 1/19/18.
//

#include "Sender.h"

bool Sender::getCommand(Robot* bot) {

    auto buffer = commUDP->getBuffer();

    if(bot->new_comm_ready) {
        for (int i = 0; i < bot->data[1]; i++) {
            buffer[i + buff_idx] = bot->data[i];
        }
        buff_idx += bot->data[1];
        bot->new_comm_ready = false;
        return true;
    }

    return false;
}

bool Sender::appendData(unsigned char* data,int length){
    auto buffer = commUDP->getBuffer();

    for (int i = 0; i < length; i++) {
        buffer[i + buff_idx] = data[i];
    }
    buff_idx += length;
    return true;
}

bool Sender::sendAll() {
    if(startup > 0){
        startup--;
        return false;
    }

    if (commUDP->send(buff_idx, NetworkAddress{ "224.5.92.5" , 60005 }))
    {
        buff_idx = 0;
        return true;
    }
    else
    {
        std::cout << "ERROR: failed to send robot packets." << std::endl;
        buff_idx = 0;
        return false;
    }

    buff_idx = 0;
    return true;
}

Sender::Sender() {
    commUDP = std::make_shared<UdpServer>();

    buff_idx = 0;
    auto buffer = commUDP->getBuffer();
    for(int i=0;i<buffer.size();i++){
        buffer[i]=0x00;
    }
    startup = 5;
}

void Sender::append_demo_data() {
    auto buffer = commUDP->getBuffer();
    buffer[0 + buff_idx] = 25;
    buffer[1 + buff_idx] = 0x0A;
    buffer[2 + buff_idx] = 0x00;
    buffer[3 + buff_idx] = 0x00;
    buffer[4 + buff_idx] = 0x00;
    buffer[5 + buff_idx] = 0x00;
    buffer[6 + buff_idx] = 0x00;
    buffer[7 + buff_idx] = 0x00;
    buffer[8 + buff_idx] = 0x00;
    buffer[9 + buff_idx] = 0x00;
    buff_idx += 10;
}
