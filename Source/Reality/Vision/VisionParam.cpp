//
// Created by dot_blue on 12/5/18.
//

#include "Vision.h"


void VisionModule::ProcessParam( WorldState * state ) {
    double avg = 0;
    int count = 0;
    for ( int i = 0 ; i < Setting::kCamCount ; i ++ ) {
        if (setting().use_camera[i]) {
            avg += frame[i].t_capture();
            count++;
        }
    }

    avg /= count;
    state->delta_t_capture = avg - state->t_capture;
    state->t_capture = avg;
}



