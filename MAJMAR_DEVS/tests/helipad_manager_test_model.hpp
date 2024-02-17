#ifndef __HELIPAD_MANAGER_TEST_MODEL_HPP__
#define __HELIPAD_MANAGER_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>

// We include any models that are directly contained within this coupled model
#include "../atomic_models/helipadManager.hpp"
#include "../data_structures/heloInfo.hpp"

namespace cadmium::assignment1 {
    class helipad_manager_test_model : public Coupled {
        public:
        helipad_manager_test_model(const std::string& id): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto helipadManager = addComponent<HelipadManager>("HelipadManager", 0.25*60);

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
            auto helipadManagerInputHQ = addComponent<cadmium::lib::IEStream<HeloInfo>>("HelipadManagerInputHQ","../input_data/helipad_manager_inHQ_test_input.txt");
			auto helipadManagerInputEM = addComponent<cadmium::lib::IEStream<bool>>("HelipadManagerInputEM","../input_data/helipad_manager_inEM_test_input.txt");

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(helipadManagerInputHQ->out,helipadManager->inHQ);
			addCoupling(helipadManagerInputEM->out,helipadManager->inEM);
        }
    };
} // namespace cadmium::assignment1

#endif // __HELIPAD_MANAGER_TEST_MODEL_HPP__
