#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <chrono>

int main() {
	std::vector<int> dimensions = {10, 50, 100, 500, 1000, 2000, 5000};
	int numPoints = 100;
	
	for (int d : dimensions) {
		std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_real_distribution<double> dis(0.0, 1.0);
		
		std::vector<std::vector<double>> points;
		
		for (int i = 0; i < numPoints; ++i) {
			std::vector<double> point;
			for (int j = 0; j < d; ++j) {
				double randomValue = dis(gen);
				point.push_back(randomValue);
			}
			points.push_back(point);
		}
		
		std::string filename = "points_dimension_" + std::to_string(d) + ".txt";
		std::ofstream outputFile(filename, std::ios::trunc);  
		if (outputFile.is_open()) {
			for (const auto& point : points) {
				for (const double value : point) {
					outputFile << value << " ";
				}
				outputFile << std::endl;
			}
			outputFile.close();
			std::cout << "Archivo " << filename << " creado exitosamente." << std::endl;
		} else {
			std::cout << "No se pudo abrir el archivo " << filename << "." << std::endl;
		}
		
		std::cout << std::endl;
	}
	
	return 0;
}
