#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Matrix.h"

class NeuralNetwork
{
public:
	NeuralNetwork();
	NeuralNetwork(int numInputs, int numHidden, int numOutputs);

private:
	//vars
	int NumInputs;
	int NumHidden;
	int NumOutputs;

	Matrix WInputToHidden;
	Matrix WHiddenToOutput;
	Matrix HiddenBias;
	Matrix OutputBias;
public:
	//get and set
	std::vector<float> getWeightsAsDNA();
	void setWeightsFromDNA(std::vector<float> DNA);
	//functions
	std::vector<float> feedForward(std::vector<float> inputs);

	~NeuralNetwork();
};

