//
// Created by dot_blue on 5/29/18.
//

#ifndef IMMORTALSSSL_CONTROLBLOCK_H
#define IMMORTALSSSL_CONTROLBLOCK_H

#include <algorithm>
#include "deque"

using namespace std;

class median{
private:
    deque<float> buff;
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
        deque<float> temp=buff;
        sort(temp.begin(), temp.end(),
                  [](float const &a, float const &b) {
                      return a < b;
                  }
        );
        int buff_size;
        if(buff_size = temp.size() > 0)
            return temp.at(buff_size/len);
        else
            return 0.0;
    }

};




#endif //IMMORTALSSSL_CONTROLBLOCK_H
