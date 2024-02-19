#ifndef __FOL_TEST_MODEL_HPP__
#define __FOL_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../atomic_models/fol.hpp"
#include "../../data_structures/evacInfo.hpp"

namespace cadmium::assignment1 {
    class fol_test_model : public Coupled {
        public:
        fol_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto fol = addComponent<FOL>("fol");

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
			std::string filepath = "../../input_data/fol_test" + testNumber + "_input.txt";
            auto folInput = addComponent<cadmium::lib::IEStream<EvacInfo>>("folInput", filepath.c_str());

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(folInput->out,fol->in);

        }
    };
} // namespace cadmium::assignment1

#endif // __FOL_TEST_MODEL_HPP__
