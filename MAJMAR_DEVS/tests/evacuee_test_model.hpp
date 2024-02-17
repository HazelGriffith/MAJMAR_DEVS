#ifndef __EVACUEE_TEST_MODEL_HPP__
#define __EVACUEE_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>

// We include any models that are directly contained within this coupled model
#include "../atomic_models/evacuee.hpp"
#include "../data_structures/evacInfo.hpp"
#include "../data_structures/heloInfo.hpp"

namespace cadmium::assignment1 {
    class evacuee_test_model : public Coupled {
        public:
        evacuee_test_model(const std::string& id): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto evacuee = addComponent<Evacuee>("Evacuee", 1);

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
            auto evacueeInput = addComponent<cadmium::lib::IEStream<EvacInfo>>("EvacueeInput","../input_data/evacuee_in_test_input.txt");

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(evacueeInput->out,evacuee->in);
        }
    };
} // namespace cadmium::assignment1

#endif // __EVACUEE_TEST_MODEL_HPP__
