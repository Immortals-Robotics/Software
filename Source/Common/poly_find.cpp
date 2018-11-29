//
// Created by dot_blue on 26/01/17.
//

#include <cmath>
#include "poly_find.h"

poly_find::poly_find(int degree):deg(degree) {

    result=new double[degree+1];
    X=new double[2*degree+1];
    Y=new double[degree+1];
    data_count=0;

    for(int i=0;i<2*degree+1;i++){
        X[i]=0;
    }
    for(int i=0;i<degree+1;i++){
        Y[i]=0;
    }
}

void poly_find::add_point(double pos_X, double pos_Y) {
    _X.push_back(pos_X);
    _Y.push_back(pos_Y);
    data_count++;

    for (int i=0;i<2*deg+1;i++)
    {
        X[i]+=pow(pos_X,i);        //consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    }
    for (int i=0;i<deg+1;i++)
    {
        Y[i]+=pow(pos_X,i)*pos_Y;        //consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    }

}

void poly_find::calculate() {
    if(data_count>3) {
        int i, j, k;
        int n = deg;

        double B[n + 1][n + 2];
        for (i = 0; i <= n; i++)
            for (j = 0; j <= n; j++)
                B[i][j] = X[i + j];
        for (i = 0; i <= n; i++)
            B[i][n +1] = Y[i];                //load the values of Y as the last column of B(Normal Matrix but augmented)
        n++;

        for (i = 0; i <
                    n; i++)                    //From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
            for (k = i + 1; k < n; k++)
                if (B[i][i] < B[k][i])
                    for (j = 0; j <= n; j++) {
                        double temp = B[i][j];
                        B[i][j] = B[k][j];
                        B[k][j] = temp;
                    }

        for (i = 0; i < n - 1; i++)            //loop to perform the gauss elimination
            for (k = i + 1; k < n; k++) {
                double t = B[k][i] / B[i][i];
                for (j = 0; j <= n; j++)
                    B[k][j] = B[k][j] - t * B[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
            }
        for (i = n - 1; i >= 0; i--)                //back-substitution
        {                        //x is an array whose values correspond to the values of x,y,z..
            result[i] = B[i][n];                //make the variable to be calculated equal to the rhs of the last equation
            for (j = 0; j < n; j++)
                if (j !=i)            //then subtract all the lhs values except the coefficient of the variable whose value                                   is being calculated
                    result[i] = result[i] - B[i][j] * result[j];
            result[i] = result[i] / B[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
        }
    }
}

void poly_find::print_res() {
    for (int i=0;i<=deg;i++)
        std::cout<<" + ("<<result[i]<<")"<<"*(A1^"<<i<<")";
    std::cout<<"\ntimes of input: "<<data_count<<std::endl;
    if(is_valid()) {
        std::cout << "and data is valid"<<std::endl;
    }else{
        std::cout << "and data is NOT valid!!!"<<std::endl;
    }
}

void poly_find::reset() {
    data_count=0;

    for(int i=0;i<2*deg+1;i++){
        X[i]=0;
    }
    for(int i=0;i<deg+1;i++){
        Y[i]=0;
    }

}

double poly_find::give_Y_by_X(double _X) {
    double out=result[0];
    for(int i=1;i<=deg;i++){
        out+=result[i]*pow(_X,i);
    }
    return out;
}

double poly_find::give_derivedY_by_X(double _X) {
    double out=result[1];
    for(int i=2;i<=deg;i++){
        out+=result[i]*pow(_X,i-1)*i;
    }
    return out;
}

double poly_find::give_X_by_Y(double _Y,double start) {
    double x,x1; //user input value
    double f,dfdx; //function and its derivative
    const double epsilon = 0.0001; //tolerance
    const int n = 1500; //max number of steps
    result[0]-=_Y;
    x=start;

    for(int i = 2; i <= n; i++)
    {
        f = give_Y_by_X(x); //Defines the function
        dfdx = give_derivedY_by_X(x); //Derivative of the function
        x = x - f/dfdx; //Newton Method formula
    }
    result[0]+=_Y;
    std::cout<<"K: "<<give_Y_by_X(x)/give_derivedY_by_X(x)<<std::endl;
    return x;

}

void poly_find::set_result(double *res) {
    for(int i=0;i<=deg;i++)
        result[i]=res[i];
}

bool poly_find::is_valid() {
    bool out=false;
    for(int i=0;i<=deg;i++){
        if(result[i]>+1.0e-30&&result[i]<+1.0e+30)
            out=true;
    }
    if(data_count<=3)
        out= false;
    return out;
}

int poly_find::give_data_count() {
    return data_count;
}

poly_find::~poly_find() {
    delete[] result;
    delete[] X;
    delete[] Y;
}

