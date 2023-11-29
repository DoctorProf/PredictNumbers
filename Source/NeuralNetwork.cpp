#include "../Headers/NeuralNetwork.h"

double NeuralNetwork::activate(double x)
{
	return 1.0 / (1.0 + exp(-x));
	//return x > 0 ? x : 0.0;
}
double NeuralNetwork::derivative(double x)
{
	return x * (1 - x);
	//return x < 0 ? 0.0 : 1.0;
}
void NeuralNetwork::generateNeurons()
{
	this->layers.resize(hideNeurons.size() + 2);
	// ��������� �������� ��������
	for (int i = 0; i < this->outputNeurons; i++)
	{
		layers[layers.size() - 1].push_back(Neuron());
	}
	// ��������� ������� ����� � ������� ��������
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
	// ��������� ������� ��������
	for (int i = 0; i < inputNeurons; i++)
	{
		layers[0].push_back(Neuron(layers[1].size()));
	}
	if (useBias) layers[0].push_back(Neuron(layers[1].size(), 1, true));
}


void NeuralNetwork::forwardPropagation(std::vector<double> inputs)
{
	// ��������� ������� ����������
	for (int i = 0; i < layers[0].size() - this->useBias; i++)
	{
		layers[0][i].setValue(inputs[i]);
	}
	// ����� ���� ��� ��������� ����� (������� ��������)
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
	forwardPropagation(inputs);
	// ������� ������ � ��������� �������� ��������
	for (int i = 0; i < layers[layers.size() - 1].size(); i++)
	{
		layers[layers.size() - 1][i].setGradient((value[i] - layers[layers.size() - 1][i].getValue()) * derivative(layers[layers.size() - 1][i].getValue()));
	}
	// ������ ��� ������� �����
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
	// ��������������� ����� ��� ���� �����
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
double NeuralNetwork::getErrorSquare(int index)
{
	if (outputNeurons == 0) return 0;
	return pow(layers[layers.size() - 1][index].getError(), 2);
}
void NeuralNetwork::saveNN()
{
	std::ofstream file("Weights.txt");
	file << useBias << std::endl;
	file << momentNes << std::endl;
	file << learningRate << std::endl;
	file << inputNeurons << std::endl;
	file << hideNeurons.size() << std::endl;
	for (int i = 0; i < hideNeurons.size(); i++) 
	{
		file << hideNeurons[i] << std::endl;
	}
	file << outputNeurons << std::endl;
	for (int i = 0; i < layers.size(); i++)
	{
		for (int j = 0; j < layers[i].size(); j++)
		{
			for (int k = 0; k < layers[i][j].getWeights().size(); k++)
			{
				file << layers[i][j].getWeights()[k] << std::endl;
			}
		}
	}
	file.close();
	std::cout << "save\n";
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
NeuralNetwork::NeuralNetwork(std::string file)
{
	std::ifstream inputFile(file);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(inputFile, line)) 
	{
		lines.push_back(line);
	}
	this->useBias = std::stoi(lines[0]);
	this->momentNes = std::stod(lines[1]);
	this->learningRate = std::stod(lines[2]);
	this->inputNeurons = std::stoi(lines[3]);
	for (int i = 0; i < std::stoi(lines[4]); i++)
	{
		hideNeurons.push_back(std::stoi(lines[5 + i]));
	}
	this->outputNeurons = std::stoi(lines[std::stoi(lines[4]) + 5]);
	generateNeurons();
	for (int i = 0; i < layers.size() - 1; i++) 
	{
		for (int j = 0; j < layers[i].size(); j++) 
		{
			for (int k = 0; k < layers[i + 1].size(); k++) 
			{
				int index = 0;
				if (i == 0) 
				{
					index = hideNeurons.size() + 6 + k + j * layers[i][0].getWeights().size();
				}
				else 
				{
					index = hideNeurons.size() + 6 + k + j * layers[i][0].getWeights().size() + i * layers[i - 1].size() * layers[i - 1][0].getWeights().size();
				}
				layers[i][j].setWeights(k, std::stod(lines[index]));
			}
		}
	}
	
}
std::vector<std::vector<Neuron>> NeuralNetwork::getLayers()
{
	return layers;
}

void NeuralNetwork::trainToIterarion(std::vector<std::vector<double>> inputSet, std::vector<std::vector<double>> outputSet, int iteration, bool save)
{
	for (int i = 0; i < iteration; i++)
	{
		for (int j = 0; j < inputSet.size(); j++)
		{
			backPropagation(inputSet[j], outputSet[j]);
		}
		printResultTrain(inputSet, outputSet);
	}
	if (save) saveNN();
}
void NeuralNetwork::trainBeforeTheError(std::vector<std::vector<double>> inputSet, std::vector<std::vector<double>> outputSet, double errorMax, int maxIteration, bool save)
{
	double error = (double)INFINITY;
	int i = 0;
	while (error > errorMax)
	{
		double sum = 0;
		std::vector<double> errors;
		for (int j = 0; j < inputSet.size(); j++)
		{
			backPropagation(inputSet[j], outputSet[j]);
			for (int k = 0; k < outputNeurons; k++)
			{
				sum += getErrorSquare(k);
			}
		}
		error = sum / outputNeurons;
		if (i > maxIteration)
		{
			std::cout << "Over iteration\n";
			break;
		}
		i++;
	}
	std::cout << "Iteration " << i << "\n";
	if (save) saveNN();
}
void NeuralNetwork::printResultTrain(std::vector<std::vector<double>> inputSet, std::vector<std::vector<double>> outputSet)
{
	int learned = 0;
	int not_learned = 0;
	int count = 0;
	for (int i = 0; i < inputSet.size(); i++)
	{
		forwardPropagation(inputSet[i]);
		for (int j = 0; j < layers[layers.size() - 1].size(); j++)
		{
			if (std::round(layers[layers.size() - 1][j].getValue()) != std::round(outputSet[i][j])) 
			{
				count++;
				//break;
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
	//std::cout << "Huinya: " << (inputSet[0] == inputSet[1]) << "\n";
	std::cout << "not LEARN " << not_learned << "\n";
	std::cout << "LEARN " << learned << "\n\n";
}
void NeuralNetwork::predict(std::vector<double> input)
{
	forwardPropagation(input);
}
