//
// Created by server on 1/19/18.
//

#include "Sender.h"

bool Sender::getCommand(Robot* bot) {

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
    for (int i = 0; i < length; i++) {
        buffer[i + buff_idx] = data[i];
    }
    buff_idx += length;
    return true;
}

bool Sender::sendAll() {

    try {
        commUDP.sendTo ( buffer    , buff_idx , "224.5.92.5" , 60005 );
    } catch (...) {
        cout << "ERROR: failed to send robot packets." << endl;
        buff_idx = 0;
        return false;
    }

    buff_idx = 0;
    return true;
}

Sender::Sender() {
    buff_idx = 0;
    for(int i=0;i<MAX_BUFF_LEN;i++){
        buffer[i]=0x00;
    }
}

void Sender::append_demo_data() {
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
