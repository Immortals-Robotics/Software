#include "ANN.h"
#include "doublefann.h"

class motionANN
{
	motionANN();
public:
	void init_ANN();
	void run_ANN(double teta , double velocity ,double * result);
	void train_ANN(double cur_teta , double cur_velocity , double cur_Omega , double des_teta , double des_velocity);
	void clear_ANN();
	
private:
	
	struct fann * ann;
	double input[2];
	fann_type * output;
	
};