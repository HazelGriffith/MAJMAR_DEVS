#include <core/simulation/root_coordinator.hpp>
#include <limits>
#include <core/logger/csv.hpp>
#include <iostream>
#include <string>


// We must include our "top model" which is a coupled model used to
// that hold other models inside of it
#include "top_model.hpp"

using namespace cadmium::assignment1;

int main(int argc,char* argv[]){

	std::string replicationName;
	int numOfHelos = 1;
	int shipArrivalTime = 200000;
	if (argc>4){
		assert(("Too many arguments", false));
	} else if (argc>1){
		std::string argument1(argv[1]);
		replicationName = "R" + argument1;
		std::string argument2(argv[2]);
		shipArrivalTime = std::stoi(argument2);
		std::string argument3(argv[3]);
		numOfHelos = std::stoi(argument3);
	} else {
		std::string filename = "top_model";
	}
	
    // Declare and initialize the top model
    auto model = std::make_shared<Top_model>("top_model", shipArrivalTime, numOfHelos);
    auto rootCoordinator = cadmium::RootCoordinator(model);
	
    // For simulation purposes, set the name of the output file
	auto logger = std::make_shared<cadmium::CSVLogger>("../simulation_results/Exp1V3/" + to_string(numOfHelos) + " Helos/ship@" + to_string(shipArrivalTime) + "h/" + replicationName + ".csv",",");
	rootCoordinator.setLogger(logger);

    rootCoordinator.start();

    // For simulations, we can set the number of seconds we want to simulate
    rootCoordinator.simulate(14400.0);

    rootCoordinator.stop();
    return 0;
}
