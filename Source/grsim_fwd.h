//
// Created by Ali Salehi on 16.06.18.
//

#ifndef IMMORTALSSSL_GRSIM_FWD_H
#define IMMORTALSSSL_GRSIM_FWD_H

#include "Soccer/ai09/Robot.h"
#include "Network/PracticalSocket.h"
#include <string.h>

class GrsimForwarder
{
private:
    UDPSocket *socket;
    std::string ip;
    short port;

public:
    GrsimForwarder(const char *const ip, const short port);
    ~GrsimForwarder();
    void SendData(const Robot *const robots, const int robot_count, bool color);
};

#endif //IMMORTALSSSL_GRSIM_FWD_H
