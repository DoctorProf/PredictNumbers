#include "../Headers/NeuralNetwork.h"

double NeuralNetwork::activate(double x)
{
	return 1.0 / (1.0 + exp(-x));
}
double NeuralNetwork::derivative(double x)
{
	return x * (1 - x);
}
void NeuralNetwork::generateNeurons()
{
	this->layers.resize(hideNeurons.size() + 2);
	// Генерация выходных нейронов
	for (int i = 0; i < this->outputNeurons; i++)
	{
		layers[layers.size() - 1].push_back(Neuron());
	}
	// Генерация скрытых слоев и скрытых нейронов
	for (int i = this->hideNeurons.size(); i > 0; i--)
	{
		if (hideNeurons[i - 1] == 0)
		{
			hideNeurons.erase(hideNeurons.begin() + i - 1);
			layers.erase(layers.begin() + i);
			continue;
		}
		for (int j = 0; j < hideNeurons[i - 1]; j++)
		{
			layers[i].push_back(Neuron(layers[i + 1].size()));
		}
		if (useBias)
		{
			layers[i].push_back(Neuron(layers[i + 1].size(), 1, true));
		}
	}
	// Генерация входных нейронов
	for (int i = 0; i < inputNeurons; i++)
	{
		layers[0].push_back(Neuron(layers[1].size()));
	}
	if (useBias) layers[0].push_back(Neuron(layers[1].size(), 1, true));
}


void NeuralNetwork::forwardFeed(std::vector<double> inputs)
{
	// Установка входных параметров
	for (int i = 0; i < layers[0].size() - this->useBias; i++)
	{
		layers[0][i].setValue(inputs[i]);
	}
	// Поиск сумм для остальных слоев (включая выходной)
	for (int i = 1; i < layers.size(); i++)
	{
		for (int j = 0; j < layers[i].size(); j++)
		{
			if (layers[i][j].getBias())
			{
				layers[i][j].setValue(1);
				continue;
			}
			double sum = 0.0;
			for (int k = 0; k < layers[i - 1].size(); k++)
			{
				sum += layers[i - 1][k].getValue() * layers[i - 1][k].getWeights()[j];
			}
			layers[i][j].setValue(activate(sum));
		}
	}
}

void NeuralNetwork::backPropagation(std::vector<double> inputs, std::vector<double> value)
{
	forwardFeed(inputs);
	// Рассчет ошибки и градиента выходных нейронов
	for (int i = 0; i < layers[layers.size() - 1].size(); i++)
	{
		layers[layers.size() - 1][i].setGradient((value[i] - layers[layers.size() - 1][i].getValue()) * derivative(layers[layers.size() - 1][i].getValue()));
	}
	// Ошибки для скрытых слоев
	for (int i = layers.size() - 2; i > 0; i--)
	{
		for (int j = 0; j < layers[i].size(); j++)
		{
			double sum = 0.0;
			for (int k = 0; k < layers[i + 1].size(); k++)
			{
				sum += layers[i + 1][k].getGradient() * layers[i][j].getWeights()[k];
			}
			layers[i][j].setGradient(sum * derivative(layers[i][j].getValue()));
		}
	}
	// Корректирование весов для всех слоев
	for (int i = 0; i < layers.size() - 1; i++)
	{
		for (int j = 0; j < layers[i].size(); j++)
		{
			for (int k = 0; k < layers[i + 1].size(); k++)
			{
				double deltaW = learningRate * layers[i + 1][k].getGradient() * layers[i][j].getValue() + momentNes * layers[i][j].getLastDeltaWeights()[k];
				layers[i][j].setWeights(k, layers[i][j].getWeights()[k] + deltaW);
				layers[i][j].setLastDeltaWeights(k, deltaW);
			}
		}
	}
}
void NeuralNetwork::initializeWeights()
{
	boost::random::mt19937 rng;
	boost::random::uniform_real_distribution<> dist(-1, 1);
	for (int i = 0; i < layers.size(); i++)
	{
		for (int j = 0; j < layers[i].size(); j++)
		{
			for (int k = 0; k < layers[i][j].getWeights().size(); k++) 
			{
				double x = dist(rng);
				layers[i][j].setWeights(k, dist(rng));
				layers[i][j].setLastDeltaWeights(k, 0);
			}
		}
	}
}
NeuralNetwork::NeuralNetwork(int inputNeurons, std::vector<int> hideNeurons, int outputNeurons, double learningRate, double momentNes, bool useBias)
{
	this->inputNeurons = inputNeurons;
	this->hideNeurons = hideNeurons;
	this->outputNeurons = outputNeurons;

	this->learningRate = learningRate;
	this->momentNes = momentNes;
	this->useBias = useBias;

	generateNeurons();
	initializeWeights();
}
std::vector<std::vector<Neuron>> NeuralNetwork::getLayers()
{
	return layers;
}

void NeuralNetwork::trainToIterarion(std::vector<std::vector<double>> inputSet, std::vector<std::vector<double>> outputSet, int iteration)
{
	for (int i = 0; i < iteration; i++)
	{
		for (int j = 0; j < inputSet.size(); j++)
		{
			backPropagation(inputSet[j], outputSet[j]);
		}
		printResultTrain(inputSet, outputSet);
	}
}
void NeuralNetwork::printResultTrain(std::vector<std::vector<double>> inputSet, std::vector<std::vector<double>> outputSet)
{
	int learned = 0;
	int not_learned = 0;
	int count = 0;
	for (int i = 0; i < inputSet.size(); i++)
	{
		forwardFeed(inputSet[i]);
		for (int j = 0; j < layers[layers.size() - 1].size(); j++)
		{
			if (std::round(layers[layers.size() - 1][j].getValue()) != std::round(outputSet[i][j])) 
			{
				count++;
			}
			std::cout << i + 1 << " NeuralNetwork: " << layers[layers.size() - 1][j].getValue() << " Waiting: " << outputSet[i][j] << "\n";
		}
		std::cout << std::endl;
		if (count == 0) {
			learned++;
		}
		else {
			not_learned++;
		}
		
		count = 0;
	}
	std::cout << "not LEARN " << not_learned << "\n";
	std::cout << "LEARN " << learned << "\n\n";
}
