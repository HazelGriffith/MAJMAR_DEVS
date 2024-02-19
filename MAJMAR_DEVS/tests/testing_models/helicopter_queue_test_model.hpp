#ifndef __HELICOPTER_QUEUE_TEST_MODEL_HPP__
#define __HELICOPTER_QUEUE_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../atomic_models/helicopterQueue.hpp"
#include "../../data_structures/heloInfo.hpp"

namespace cadmium::assignment1 {
    class helicopter_queue_test_model : public Coupled {
        public:
        helicopter_queue_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto helicopterQueue = addComponent<HelicopterQueue>("HelicopterQueue");

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
			std::string filepath1 = "../../input_data/helicopter_queue_inHelo_test" + testNumber + "_input.txt";
			std::string filepath2 = "../../input_data/helicopter_queue_inHM_test" + testNumber + "_input.txt";
			
			
            auto helicopterQueueInputHelo = addComponent<cadmium::lib::IEStream<HeloInfo>>("HelicopterQueueInputHelo",filepath1.c_str());
			auto helicopterQueueInputHM = addComponent<cadmium::lib::IEStream<bool>>("HelicopterQueueInputHM",filepath2.c_str());

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(helicopterQueueInputHelo->out,helicopterQueue->inHelo);
			addCoupling(helicopterQueueInputHM->out,helicopterQueue->inHM);
        }
    };
} // namespace cadmium::assignment1

#endif // __HELICOPTER_QUEUE_TEST_MODEL_HPP__
