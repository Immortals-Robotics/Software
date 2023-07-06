//
// Created by dot_blue on 5/29/18.
//

#ifndef IMMORTALSSSL_CONTROLBLOCK_H
#define IMMORTALSSSL_CONTROLBLOCK_H

#include <algorithm>
#include "deque"

class median{
private:
    std::deque<float> buff;
    int len;

public:

    median(int _len):len(_len) {}

    void add_data(float num){
        buff.push_back(num);
        if(buff.size()>len){
            buff.pop_front();
        }
    }

    float get_output(){
        std::deque<float> temp(buff);
        std::sort(temp.begin(), temp.end(),
                  [](float const &a, float const &b) {
                      return a < b;
                  }
        );
        int buff_size;
        if((buff_size = temp.size()) > 0)
            return temp.at(buff_size/2);
        else {
            std::cout<<"median: ERROR the buff is empty!!!"<< std::endl;
            return 0.0;
        }
    }

    void print_all(){
        std::deque<float> temp(buff);
        std::sort(temp.begin(), temp.end(),
             [](float const &a, float const &b) {
                 return a < b;
             }
        );
        for(std::deque<float>::iterator it = temp.begin();it!=temp.end();it++) {

            std::cout<<*it<<"_";

        }
        std::cout<< std::endl;
        for(int i=0;i<temp.size();i++){
            std::cout<<temp.at(i)<<'=';
        }
        std::cout<< std::endl;
    }

};




#endif //IMMORTALSSSL_CONTROLBLOCK_H
