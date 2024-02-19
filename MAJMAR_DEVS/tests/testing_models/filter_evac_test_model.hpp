#ifndef __FILTER_EVAC_TEST_MODEL_HPP__
#define __FILTER_EVAC_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../atomic_models/filterEvac.hpp"
#include "../../data_structures/evacInfo.hpp"

namespace cadmium::assignment1 {
    class filter_evac_test_model : public Coupled {
        public:
        filter_evac_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto filterEvac = addComponent<FilterEvac>("filterEvac", 1);

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
			
			std::string filepath1 = "../input_data/filter_evac_test" + testNumber + "_input.txt";
            auto filterEvacInput = addComponent<cadmium::lib::IEStream<EvacInfo>>("filterEvacInput",filepath1.c_str());

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(filterEvacInput->out,filterEvac->in);

        }
    };
} // namespace cadmium::assignment1

#endif // __FILTER_EVAC_TEST_MODEL_HPP__
