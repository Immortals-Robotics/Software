//
// Created by dot_blue on 11/29/18.
//

#ifndef IMMORTALSSSL_PLOTTER_H
#define IMMORTALSSSL_PLOTTER_H


#include "../../Network/Protobuf/data_plot.pb.h"
#include "../../Network/PracticalSocket.h"

class plotter {
public:
    data_gram bufferStream;
    data_flow* y_Blue;
    data_flow* y_Red;
    data_flow* y_Green;

    UDPSocket commUDP;
    std::string UDP_Address;
    short port_Address;

    plotter(const char *const ip, const short port_num){
        UDP_Address = ip;
        port_Address = port_num;

        y_Blue = bufferStream.add_data_package();
        y_Blue -> set_name("BLUE DATA");

        y_Red = bufferStream.add_data_package();
        y_Red -> set_name("RED DATA");

        y_Green = bufferStream.add_data_package();
        y_Green -> set_name("GREEN DATA");
    }
    void BLUE_pushData(float data);
    void RED_pushData(float data);
    void GREEN_pushData(float data);

    void send_data();
};


#endif //IMMORTALSSSL_PLOTTER_H
