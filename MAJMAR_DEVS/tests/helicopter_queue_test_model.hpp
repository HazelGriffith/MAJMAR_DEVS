#ifndef __HELICOPTER_QUEUE_TEST_MODEL_HPP__
#define __HELICOPTER_QUEUE_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>

// We include any models that are directly contained within this coupled model
#include "../atomic_models/helicopterQueue.hpp"
#include "../data_structures/heloInfo.hpp"

namespace cadmium::assignment1 {
    class helicopter_queue_test_model : public Coupled {
        public:
        helicopter_queue_test_model(const std::string& id): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto helicopterQueue = addComponent<HelicopterQueue>("HelicopterQueue");

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
            auto helicopterQueueInputHelo = addComponent<cadmium::lib::IEStream<HeloInfo>>("HelicopterQueueInputHelo","../input_data/helicopter_queue_inHelo_test_input.txt");
			auto helicopterQueueInputHM = addComponent<cadmium::lib::IEStream<bool>>("HelicopterQueueInputHM","../input_data/helicopter_queue_inHM_test_input.txt");

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(helicopterQueueInputHelo->out,helicopterQueue->inHelo);
			addCoupling(helicopterQueueInputHM->out,helicopterQueue->inHM);
        }
    };
} // namespace cadmium::assignment1

#endif // __HELICOPTER_QUEUE_TEST_MODEL_HPP__
