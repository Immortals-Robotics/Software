//
// Created by dot_blue on 26/01/17.
//

#ifndef QTGRAPHICTEST_POLY_FIND_H
#define QTGRAPHICTEST_POLY_FIND_H

#include <vector>
#include <iostream>

class poly_find {
public:
    poly_find(int degree);
    ~poly_find();
    void add_point(double pos_X, double pos_Y);
    void calculate();
    void print_res();
    void reset();
    double give_X_by_Y(double _Y,double start);
    double give_Y_by_X(double _X);
    double give_derivedY_by_X(double _X);
    void set_result(double res[]);
    bool is_valid();
    int give_data_count();

private:
    int deg;
    std::vector<double> _X,_Y;
    int data_count;
    double* result;
    double* X;
    double* Y;

};


#endif //QTGRAPHICTEST_POLY_FIND_H
