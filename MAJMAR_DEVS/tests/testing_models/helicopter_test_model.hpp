#ifndef __HELICOPTER_TEST_MODEL_HPP__
#define __HELICOPTER_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../atomic_models/helicopter.hpp"
#include "../../data_structures/evacInfo.hpp"
#include "../../data_structures/heloInfo.hpp"

namespace cadmium::assignment1 {
    class helicopter_test_model : public Coupled {
        public:
        helicopter_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto helicopter = addComponent<Helicopter>("Helicopter", 1);

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
			std::string filepath1 = "../input_data/helicopter_inEvac_test" + testNumber + "_input.txt";
			std::string filepath2 = "../input_data/helicopter_inES_test" + testNumber + "_input.txt";
			
			
            auto helicopterInputEvac = addComponent<cadmium::lib::IEStream<EvacInfo>>("HelicopterInputEvac",filepath1.c_str());
			auto helicopterInputES = addComponent<cadmium::lib::IEStream<HeloInfo>>("HelicopterInputES",filepath2.c_str());

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(helicopterInputEvac->out,helicopter->inEvac);
			addCoupling(helicopterInputES->out,helicopter->inES);
        }
    };
} // namespace cadmium::assignment1

#endif // __HELICOPTER_TEST_MODEL_HPP__
