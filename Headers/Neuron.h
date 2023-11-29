#pragma once
#include <iostream>
#include <vector>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>

class Neuron
{
private:
	std::vector<double> weights;
	std::vector<double> lastDeltaWeights;
	bool toBias;
	double value;
	double error;
	double gradient;
public:
	Neuron(int countWeight = 0, double value = 0, bool toBias = false);
	void setValue(double value);
	double getValue();
	void setError(double error);
	double getError();
	void setGradient(double gradient);
	double getGradient();
	void setWeights(int index, double value);
	std::vector<double> getWeights();
	void setLastDeltaWeights(int index, double value);
	std::vector<double> getLastDeltaWeights();
	bool getBias();
};