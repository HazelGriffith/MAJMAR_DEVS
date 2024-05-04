#ifndef __EVACUEE_WC_TEST_MODEL_HPP__
#define __EVACUEE_WC_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../atomic_models/evacuee.hpp"
#include "../../data_structures/evacInfo.hpp"
#include "../../data_structures/heloInfo.hpp"

namespace cadmium::assignment1 {
    class evacuee_test_model : public Coupled {
        public:
        evacuee_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto evacuee = addComponent<Evacuee>("Evacuee", 1, 'W');

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
			std::string filepath1 = "../input_data/evacuee_wc_in_test" + testNumber + "_input.txt";
            auto evacueeInput = addComponent<cadmium::lib::IEStream<EvacInfo>>("EvacueeInput",filepath1.c_str());

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(evacueeInput->out,evacuee->in);
        }
    };
} // namespace cadmium::assignment1

#endif // __EVACUEE_WC_TEST_MODEL_HPP__
