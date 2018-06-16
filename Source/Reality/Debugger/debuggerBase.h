//
// Created by dot_blue on 5/16/18.
//

#ifndef IMMORTALSSSL_DEBUGGERBASE_H
#define IMMORTALSSSL_DEBUGGERBASE_H


#define MAX_BUFF_LEN_DBG 300

#include "../../Network/PracticalSocket.h"
#include "../../Network/Protobuf/aidebug.pb.h"
#include "../../Common/GameSetting.h"

class debuggerBase {
private:
    UDPSocket commUDP;
    AI_Debug* AIDebug;

    std::string UDP_Address;
    short port_Address;
public:

    unsigned char buffer[MAX_BUFF_LEN_DBG];

    debuggerBase(GameSetting* setting,AI_Debug* _aiDebug);
    bool send();
};


#endif //IMMORTALSSSL_DEBUGGERBASE_H
