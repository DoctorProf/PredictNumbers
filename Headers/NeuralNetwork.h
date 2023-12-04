#pragma once
#include "Neuron.h"
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <string>

class NeuralNetwork
{
private:
	bool useBias;
	double momentNes;
	double learningRate;
	int inputNeurons;
	std::vector<int> hideNeurons;
	int outputNeurons;
	std::vector<std::vector<Neuron>> layers;

	double activate(double x);
	double derivative(double x);
	void generateNeurons();
	void backPropagation(std::vector<double> inputs, std::vector<double> value);
	void initializeWeights();
public:
	void forwardFeed(std::vector<double> inputs);
	NeuralNetwork(int inputNeurons, std::vector<int> hideNeurons, int outputNeurons, double learningRate, double momentNes, bool useBias);
	std::vector<std::vector<Neuron>> getLayers();
	void trainToIterarion(std::vector<std::vector<double>> inputSet, std::vector<std::vector<double>> outputSet, int iteration);
	void printResultTrain(std::vector<std::vector<double>> inputSet, std::vector<std::vector<double>> outputSet);
};