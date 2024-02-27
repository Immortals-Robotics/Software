//
// Created by dot_blue on 11/29/18.
//
#include <iostream>
#include "plotter.h"


void plotter::BLUE_pushData(float data){
    y_Blue -> add_y(data);
}

void plotter::RED_pushData(float data) {
    y_Red -> add_y(data);
}

void plotter::GREEN_pushData(float data) {
    y_Green -> add_y(data);
}

void plotter::send_data() {
    std::string* tempSTR = new std::string();
    try {
        bufferStream.SerializeToString(tempSTR);
        commUDP.sendTo ( tempSTR->c_str()    , tempSTR->length() , this->UDP_Address , this->port_Address );
        std::cout<<"The DATA length was: "<<tempSTR->length()<< std::endl;
    } catch (...) {
        std::cout << "ERROR: failed to send plot packets." << std::endl;
    }
    y_Blue -> Clear();
    y_Red -> Clear();
    y_Green -> Clear();

}





