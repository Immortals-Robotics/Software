//
// Created by server on 1/19/18.
//

#ifndef CALIBENGINE_SENDER_H
#define CALIBENGINE_SENDER_H
#define MAX_BUFF_LEN    500

#include "../../Soccer/ai09/Robot.h"

//Network
#include "../../Network/PracticalSocket.h"


class Sender {
private:
    UDPSocket commUDP;

public:

    unsigned char buffer[MAX_BUFF_LEN];
    int buff_idx;
    int startup;

    bool getCommand(Robot* bot);
    bool appendData(unsigned char* data,int length);
    bool sendAll();
    void append_demo_data();

    Sender();
};

#endif //CALIBENGINE_SENDER_H
