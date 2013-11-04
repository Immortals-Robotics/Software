#include "motionANN.h"


void motionANN::init_ANN()
{
	ann = fann_create_standard(3,2,5,3);
	//ann = fann_create_from_file("./ANN.net");
	fann_set_activation_function_hidden(ann, FANN_SIGMOID);	
	fann_set_activation_function_output(ann, FANN_SIGMOID);
	fann_set_training_algorithm(ann , FANN_TRAIN_INCREMENTAL);
	fann_set_learning_rate(ann , 0.1);	
}

void motionANN::run_ANN(double teta , double velocity ,double * result)
{
	
	input[0] = teta;
	input[1] = velocity;
	output = fann_run(ann,input);
	result[0] = output[0]; // teta
	result[1] = output[1]; // velocity
	result[2] = output[2]; // omega
	
}

void motionANN::train_ANN(double cur_teta , double cur_velocity , double cur_Omega , double des_teta , double des_velocity)
{
	
	double input[2];
	double trainData[2];
	
	input[0] = des_teta;
	input[1] = des_velocity;
	trainData[0] = cur_teta;
	trainData[1] = cur_velocity;
	trainData[2] = cur_Omega;
	
	fann_train(ann,input,trainData);
	fann_save(ann , "./ANN.net");

	
}

void motionANN::clear_ANN()
{
	
}