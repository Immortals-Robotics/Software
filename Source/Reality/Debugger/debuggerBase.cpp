//
// Created by dot_blue on 5/16/18.
//

#include <iostream>
#include "debuggerBase.h"

debuggerBase::debuggerBase(GameSetting* setting,AI_Debug *_aiDebug):AIDebug(_aiDebug) {
    UDP_Address = setting->GUI_UDP_Address;
    port_Address = setting->GUIPort;
}

bool debuggerBase::send() {
    string* tempSTR=new string;
    try {
        AIDebug->SerializeToString(tempSTR);
        commUDP.sendTo ( tempSTR->c_str()    , tempSTR->length() , UDP_Address , port_Address );
//        cout<<"sent GUI"<<endl;
    } catch (...) {
        cout << "ERROR: failed to send robot packets." << endl;
        return false;
    }
    return true;
}
