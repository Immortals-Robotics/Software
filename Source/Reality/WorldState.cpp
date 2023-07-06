//
// Created by dot_blue on 10/23/18.
//
#include "WorldState.h"
#include <iostream>
#include <fstream>

sample_tuple tmp_sample;

double first_t = -1.0;

void write_to_file(){
    std::ofstream myfile;

    std::string sample_path(DATA_DIR);
    sample_path.append("/samples.txt");

    myfile.open(sample_path, std::ios::app);//append

    myfile << tmp_sample.t << " "
           << tmp_sample.X << " "
           << tmp_sample.Y << " "
           << tmp_sample.VX_in << " "
           << tmp_sample.VY_in << " "
           << tmp_sample.VX_out << " "
           << tmp_sample.VY_out << std::endl;

    myfile.close();
}

void get_sample_set1(double sample_t, float sample_X,float sample_Y){
    if(first_t == -1.0){
        first_t = sample_t;
    }
    tmp_sample.t = sample_t - first_t;
    tmp_sample.X = sample_X;
    tmp_sample.Y = sample_Y;
    std::cout <<"tmp_sample.t: "<< tmp_sample.t << std::endl;
}

void get_sample_set2(float sample_VX, float sample_VY){
    tmp_sample.VX_out = sample_VX;
    tmp_sample.VY_out = sample_VY;
//    write_to_file();
}

void get_sample_set3(float sample_VX, float sample_VY){
    tmp_sample.VX_in = sample_VX;
    tmp_sample.VY_in = sample_VY;
    write_to_file();
}

//-----------------------------------



