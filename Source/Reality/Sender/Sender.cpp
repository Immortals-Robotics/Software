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
        //cout<<"sent"<<endl;
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
