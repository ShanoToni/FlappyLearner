#include "NeuralNetwork.h"
#include <math.h>  
#include <iostream>

NeuralNetwork::NeuralNetwork():NeuralNetwork(0, 0, 0)
{

}
NeuralNetwork::NeuralNetwork(int numInputs, int numHidden, int numOutputs):
	NumInputs(numInputs), NumHidden(numHidden), NumOutputs(numOutputs)
{
	WInputToHidden = Matrix(NumHidden, NumInputs);
	WHiddenToOutput = Matrix(NumOutputs, NumHidden);
	HiddenBias = Matrix(NumHidden, 1);
	OutputBias = Matrix(NumOutputs, 1);
	WInputToHidden.randomize();
	WHiddenToOutput.randomize();
	HiddenBias.randomize();
	OutputBias.randomize();

}
std::vector<float> NeuralNetwork::getWeightsAsDNA()
{
	std::vector<float> InputToHiddenVec = WInputToHidden.toVector();
	std::vector<float> HiddenToOutputVec = WHiddenToOutput.toVector();
	std::vector<float> HiddenBiasVec = HiddenBias.toVector();
	std::vector<float> OutputBiasVec = OutputBias.toVector();

	std::vector<float> DNA;
	DNA.insert(DNA.begin(), InputToHiddenVec.begin(), InputToHiddenVec.end());
	DNA.insert(DNA.end(), HiddenToOutputVec.begin(), HiddenToOutputVec.end());
	DNA.insert(DNA.end(), HiddenBiasVec.begin(), HiddenBiasVec.end());
	DNA.insert(DNA.end(), OutputBiasVec.begin(), OutputBiasVec.end());

	return DNA;
}
void NeuralNetwork::setWeightsFromDNA(std::vector<float> DNA)
{
	std::vector<float> InputToHiddenVec = WInputToHidden.toVector();
	std::vector<float> HiddenToOutputVec = WHiddenToOutput.toVector();
	std::vector<float> HiddenBiasVec = HiddenBias.toVector();
	std::vector<float> OutputBiasVec = OutputBias.toVector();

	int position{ 0 };

	for (int i = 0; i < InputToHiddenVec.size(); i++)
	{
		InputToHiddenVec[i] = DNA[i];
		position = position + 1;
	}
	for (int i = 0; i < InputToHiddenVec.size(); i++)
	{
		InputToHiddenVec[i] = DNA[i+position];
		position = position + 1;
	}
	for (int i = 0; i < HiddenBiasVec.size(); i++)
	{
		HiddenBiasVec[i] = DNA[i + position];
		position = position + 1;
	}
	for (int i = 0; i < OutputBiasVec.size(); i++)
	{
		OutputBiasVec[i] = DNA[i + position];
		position = position + 1;
	}

	WInputToHidden.set(InputToHiddenVec);
	WHiddenToOutput.set(HiddenToOutputVec);
	HiddenBias.set(HiddenBiasVec);
	OutputBias.set(OutputBiasVec);
}
std::vector<float> NeuralNetwork::feedForward(std::vector<float> inputs)
{
	Matrix hidden = WInputToHidden * Matrix::fromVector(inputs);
	hidden = hidden + HiddenBias;

	auto sigmoid = [](float x) {return (1.f / (1.f + exp(-x))); };
	//activation function
	hidden.map(sigmoid);
	
	Matrix output = WHiddenToOutput * hidden;
	output = output + OutputBias;
	output.map(sigmoid);

	
	float temp = output.toVector()[0];
	std::cout << temp << std::endl;
	return output.toVector();
}

NeuralNetwork::~NeuralNetwork()
{

}
