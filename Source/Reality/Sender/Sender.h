#pragma once

#include "../../Soccer/ai09/Robot.h"

//Network
#include "../../Common/network/udp_server.h"

class Sender {
private:
    std::shared_ptr<UdpServer> commUDP;

public:

    int buff_idx;
    int startup;

    bool getCommand(Robot* bot);
    bool appendData(unsigned char* data,int length);
    bool sendAll();
    void append_demo_data();

    Sender();
};
