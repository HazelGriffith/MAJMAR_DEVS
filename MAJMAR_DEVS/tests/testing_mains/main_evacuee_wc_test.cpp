#include <core/simulation/root_coordinator.hpp>
#include <limits>
#include <core/logger/csv.hpp>
#include <cassert>
#include <string>

#include "../testing_models/evacuee_wc_test_model.hpp"

using namespace cadmium::assignment1;

int main(int argc,char* argv[]){
	std::string testNumber = "1";
	if (argc>2){
		assert(("Too many arguments", false));
	} else if (argc>1){
		testNumber = argv[1];
		if (testNumber.length() > 1){
			assert(("Not a single digit", false));
			if (!std::isdigit(testNumber.at(0))){
				assert(("Not a single digit", false));
			}
		}
	}
    // Declare and initialize the top model
    auto model = std::make_shared<evacuee_test_model>("evacuee_wc_test_model", testNumber);

    auto rootCoordinator = cadmium::RootCoordinator(model);

    // For simulation purposes, set the name of the output file
	auto logger = std::make_shared<cadmium::CSVLogger>("../simulation_results/evacuee_wc_test" + testNumber + "_model.csv",",");
	rootCoordinator.setLogger(logger);

    rootCoordinator.start();
    // For simulations, we can set the number of seconds we want to simulate
    rootCoordinator.simulate(20000.0);

    rootCoordinator.stop();
    return 0;
}
