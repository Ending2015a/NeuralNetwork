#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>

#include <cstdlib>
#include <cmath>



#include "NetworkParser.h"
#include "NeuralNetwork.h"
#include "INIParser.h"


/*
void Network_Output_Test() {
	//read network file to stringstream
	std::ifstream fin("Test/network/after.ed");
	std::stringstream ss;
	ss << fin.rdbuf();

	//parse to network
	std::ofstream fout("Test/network/after2.ed");
	std::ofstream fout2("Test/network/layout.txt");
	ednn::NetworkParser parser(ss.str());

	ednn::object o = parser.ParseToObj();
	fout2 << o << std::endl;

	fout << std::setprecision(20);
	ednn::NeuralNetwork nn = parser.Parse();
	fout << nn << std::endl;
}

void Network_Parser_Test() {
	std::ifstream fin("Test/network_parser/network1.ed");
	std::stringstream ss;
	ss << fin.rdbuf();



	ednn::NetworkParser parser(ss.str());

	ednn::object o = parser.ParseToObj();

	std::ofstream fout("Test/network_parser/layout.txt");

	fout << o << std::endl;
}

void Network_Test() {
	std::ofstream fout("stemp.txt");
	fout << std::setprecision(20);

	std::vector<std::vector<double>> datasets({
		{ 2.7810836,2.550537003,       1, 0 },
		{ 1.465489372,2.362125076,     1, 0 },
		{ 3.396561688,4.400293529,     1, 0 },
		{ 1.38807019,1.850220317,      1, 0 },
		{ 3.06407232,3.005305973,      1, 0 },
		{ 7.627531214,2.759262235,     0, 1 },
		{ 5.332441248,2.088626775,     0, 1 },
		{ 6.922596716,1.77106367,      0, 1 },
		{ 8.675418651,-0.242068655,    0, 1 },
		{ 7.673756466,3.508563011,     0, 1 }
	});


	//Create Network
	ednn::NeuralNetwork nn({ 2, 2, 2 });
	nn.CreateNetwork();
	nn.InitNetwork();

	//print out network before training
	fout << "Initial state............" << std::endl;
	fout << nn;
	fout << std::endl << std::endl;

	//Create trainer
	ednn::NeuralNetworkTrainer trainer(nn, 1000, 0.5, false);
	trainer.SetLogStream(fout);
	trainer << datasets;


	//print out network after training
	fout << "After ............" << std::endl;
	fout << nn;
	fout << std::endl << std::endl;


	for (size_t i = 0; i < datasets.size(); i++) {
		std::vector<double> output = nn << datasets[i];
		for (size_t j = 0; j < output.size(); j++) {
			fout << output[j] << " ";
		}
		fout << std::distance(output.begin(), std::max_element(output.begin(), output.end())) << std::endl;
	}
}*/


std::vector<std::vector<double>> Generate_TrainCase() {
	std::vector<std::vector<double>> testcase;

	double x, y;
	int o;
	while (std::cin >> x) {
		std::cin >> y >> o;
		testcase.push_back(std::vector<double>(
			{x, y, std::sin(x), std::sin(y), (double)(o?0:1), (double)o}));
	}

	return testcase;
}


int main(void) {

	std::ifstream fin("config.ini");
	ed::INIparser iniparser(fin);
	fin.close();
	double learning_rate = iniparser.getDouble("Training", "LEARNING_RATE", 0.0);
	double momentum = iniparser.getDouble("Training", "MOMENTUM", 0.0);
	bool shuffle = iniparser.getInt("Training", "SHUFFLE") > 0;
	int Epoch = iniparser.getInt("Training", "EPOCH", 5000);
	std::vector<int> layer = iniparser.getIntArray("Training", "LAYER");
	std::string network_file = iniparser.getString("Training", "NETWORK_FILE");
	std::string test_output_file = iniparser.getString("Training", "TEST_OUTPUT_FILE");
	int precision = iniparser.getInt("Training", "PRECISION");
	bool log = iniparser.getInt("Training", "OPEN_TRAINING_LOG") > 0;

	std::cout << "-------- Setting --------" << std::endl;
	
	std::cout << ":: Learning Rate = " << learning_rate << std::endl;
	std::cout << ":: Momentum = " << momentum << std::endl;
	std::cout << ":: Shuffle = " << (shuffle?"ON":"OFF") << std::endl;
	std::cout << ":: Epoch = " << Epoch << std::endl;
	std::cout << ":: Layer = {";
	for (size_t i = 0; i < layer.size(); i++)std::cout << layer[i] << (i < layer.size()-1 ? ", " : "}");
	std::cout << std::endl;
	std::cout << ":: Network File = " << network_file << std::endl;
	std::cout << ":: Output File = " << test_output_file << std::endl;
	std::cout << ":: Precision = " << precision << std::endl;
	std::cout << ":: Open Log = " << (log ? "ON" : "OFF") << std::endl;
	std::cout << "-------- Testing --------" << std::endl;

	/*
	//file stream
	std::ofstream fout(network_file);
	fout << std::setprecision(precision);

	//create network
	ednn::NeuralNetwork nn(layer);
	nn.CreateNetwork();
	nn.InitNetwork();

	//read testcase
	std::vector<std::vector<double>> tc = Generate_TrainCase();

	ednn::NeuralNetworkTrainer trainer(nn, Epoch, learning_rate, momentum, shuffle);
	trainer.SetLogStream(std::cout);

	trainer << tc;

	fout << nn << std::endl;*/
	
	std::ofstream fout(test_output_file);
	std::ifstream fin2(network_file);
	std::stringstream ss;
	ss << fin2.rdbuf();

	std::string str = ss.str();
	ednn::NetworkParser parser(str);
	ednn::NeuralNetwork nn = parser.Parse();

	std::vector<std::vector<double>> tc = Generate_TrainCase();

	
	std::vector<std::vector<double>> o = nn << tc;

	int correct = 0;

	for (size_t i = 0; i < o.size(); i++) {
		int idx = (int)std::distance(o[i].begin(), std::max_element(o[i].begin(), o[i].end()));
		if (idx == 0 && tc[i][4] == 1 || idx == 1 && tc[i][5] == 1)correct++;
		fout << idx << std::endl;
	}

	std::cout << "Accuracy = " << (double)correct / (double)o.size() << std::endl;

	fout.close();
	fin2.close();

	//Network_Parser_Test();
	//Network_Output_Test();
	//Network_Test();

	//system("pause");
	return 0;
}
