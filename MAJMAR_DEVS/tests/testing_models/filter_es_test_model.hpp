#ifndef __FILTER_ES_TEST_MODEL_HPP__
#define __FILTER_ES_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../atomic_models/filterES.hpp"
#include "../../data_structures/heloInfo.hpp"

namespace cadmium::assignment1 {
    class filter_es_test_model : public Coupled {
        public:
        filter_es_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto filterES = addComponent<FilterES>("filterES", 1);

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
			
			std::string filepath1 = "../../input_data/filter_es_test" + testNumber + "_input.txt";
            auto filterESInput = addComponent<cadmium::lib::IEStream<HeloInfo>>("filterESInput",filepath1.c_str());

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(filterESInput->out,filterES->in);

        }
    };
} // namespace cadmium::assignment1

#endif // __FILTER_ES_TEST_MODEL_HPP__
