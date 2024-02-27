//
// Created by dot_blue on 12/6/18.
//

#include "ai09.h"


float ai09::outOfField(TVec2 point){
    float THR = 0.0;//200.0;
    if(fabs(point.X) > field_width || fabs(point.Y) > field_height){
        return true;
    }
    return false;

}
