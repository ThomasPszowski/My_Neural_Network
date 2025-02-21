#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include "NeuralNetwork.h"

std::vector<float>& SquareVector(std::vector<float>& vec) {
	for (float& f : vec) {
		f *= f;
	}
	return vec;
}

std::vector<float>& SoftMax(std::vector<float>& vec) {
	float sum = 0;
	for (float f : vec) {
		sum += exp(f);
	}
	for (float& f : vec) {
		f = exp(f) / sum;
	}
	return vec;
}

NeuralNetwork::NeuralNetwork(std::vector<int> sizes, float (*a_func)(float)) {
	activation_function = a_func;
	for (int i = 0; i < sizes.size() - 1; i++) { // hidden layers and output layer
		layers.push_back(Layer(sizes[i + 1], sizes[i]));
	}
}

std::vector<float> NeuralNetwork::feedforward(std::vector<float> input) { 
	std::vector<float> result = input;
	for (auto& layer : layers) {
		std::vector<float> new_result(layer.size);
		for (int i = 0; i < layer.size; i++) {
			float sum = layer.biases[i];
			for (int j = 0; j < layer.weights[i].size(); j++) {
				sum += layer.weights[i][j] * result[j];
			}
			new_result[i] = activation_function(sum);
		}
		result = new_result;
	}
	return result;
}

float NeuralNetwork::cost(std::vector<std::vector<float>> data) {
	float combined_cost = 0;
	for (auto& vec : data) {
		float x = vec[0];
		float y = vec[1];
		int expected = (int)vec[2];
		std::vector<float> output_vec = feedforward({ x, y });
		SoftMax(output_vec);
		output_vec[expected] = 1 - output_vec[expected];
		SquareVector(output_vec);
		for (float output : output_vec) {
			combined_cost += output;
		}
	}
	return combined_cost;
}

void NeuralNetwork::randomizeWeights(int seed) {
	srand(seed);
	for (auto& layer : layers) {
		for (int i = 0; i < layer.size; i++) {
			layer.biases[i] = (rand() % 1000) / 1000.0 - 0.5;
			for (int j = 0; j < layer.weights[i].size(); j++) {
				layer.weights[i][j] = (rand() % 1000) / 1000.0 - 0.5;
			}
		}
	}
}

Layer::Layer(int size, int prev_size) {
	this->size = size;
	biases = std::vector<float>(size);
	weights = std::vector<std::vector<float>>(size, std::vector<float>(prev_size));
}

void NeuralNetwork::save(std::string filename) {
	std::ofstream file(filename);
	file << layers.size() + 1 << std::endl;
	file << layers[0].weights[0].size() << " ";
	for (auto& layer : layers) {
		file << layer.size << " ";
	}
	file << std::endl;
	for (auto& layer : layers) {
		for (float f : layer.biases) {
			file << f << " ";
		}
	}
	file << std::endl;
	for (auto& layer : layers) {
		for (auto& vec : layer.weights) {
			for (float f : vec) {
				file << f << " ";
			}
		}
	}
	file << std::endl;
	file.close();
}

void NeuralNetwork::load(std::string filename) {
	layers.clear();
	std::ifstream file(filename);
	std::vector<int> sizes;
	int size;
	int layers_count;
	file >> layers_count;
	for (int i = 0; i < layers_count; i++) {
		file >> size;
		sizes.push_back(size);
	}
	for (int i = 0; i < sizes.size() - 1; i++) {
		layers.push_back(Layer(sizes[i + 1], sizes[i]));
	}
	for (auto& layer : layers) {
		for (float& f : layer.biases) {
			file >> f;
		}
	}
	for (auto& layer : layers) {
		for (auto& vec : layer.weights) {
			for (float& f : vec) {
				file >> f;
			}
		}
	}
}