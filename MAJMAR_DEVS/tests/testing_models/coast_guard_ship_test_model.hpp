#ifndef __COAST_GUARD_SHIP_TEST_MODEL_HPP__
#define __COAST_GUARD_SHIP_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../atomic_models/CoastGuardShip.hpp"

using namespace std;

namespace cadmium::assignment1 {
    class coast_guard_ship_test_model : public Coupled {
        public:
        coast_guard_ship_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto cgs = addComponent<CoastGuardShip>("CoastGuardShip", 2880);

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
			std::string filepath1 = "../input_data/coast_guard_ship_in_test" + testNumber + "_input.txt";
            auto cgsInput = addComponent<cadmium::lib::IEStream<bool>>("CoastGuardShipInput",filepath1.c_str());

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(cgsInput->out,cgs->in);
        }
    };
} // namespace cadmium::assignment1

#endif // __COAST_GUARD_SHIP_TEST_MODEL_HPP__
