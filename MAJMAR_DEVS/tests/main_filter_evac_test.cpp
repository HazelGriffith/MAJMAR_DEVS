#include <core/simulation/root_coordinator.hpp>
#include <limits>
#include <core/logger/csv.hpp>


// We must include our "top model" which is a coupled model used to
// that hold other models inside of it
#include "filter_evac_test_model.hpp"

using namespace cadmium::assignment1;

int main(int argc,char* argv[]){
	
    // Declare and initialize the top model
    auto model = std::make_shared<filter_evac_test_model>("filter_evac_test_model");

    auto rootCoordinator = cadmium::RootCoordinator(model);

    // For simulation purposes, set the name of the output file
	auto logger = std::make_shared<cadmium::CSVLogger>("../simulation_results/filter_evac_test_model.csv",",");
	rootCoordinator.setLogger(logger);

    rootCoordinator.start();
    // For simulations, we can set the number of seconds we want to simulate
    rootCoordinator.simulate(100.0);

    rootCoordinator.stop();
    return 0;
}
