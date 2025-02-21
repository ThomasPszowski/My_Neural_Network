#pragma once
#include <vector>

std::vector<float>& SquareVector(std::vector<float>& vec);
std::vector<float>& SoftMax(std::vector<float>& vec);

class Layer
{
public:
	int size;
	std::vector<float> biases;
	std::vector<std::vector<float>> weights;
	Layer(int size, int prev_size);
};

class NeuralNetwork
{
public:
	std::vector<Layer> layers;
	float (*activation_function)(float);
	NeuralNetwork(std::vector<int> sizes, float (*a_func)(float));
	std::vector<float> feedforward(std::vector<float> input);
	float cost(std::vector<std::vector<float>> data);
	void randomizeWeights(int seed);
	void save(std::string filename);
	void load(std::string filename);
	

};






