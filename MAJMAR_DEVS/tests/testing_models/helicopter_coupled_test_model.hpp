#ifndef __HELICOPTER_COUPLED_TEST_MODEL_HPP__
#define __HELICOPTER_COUPLED_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../coupled_models/HelicopterCoupledModel.hpp"

// data structures
#include "../../data_structures/heloInfo.hpp"
#include "../../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
    class helicopter_coupled_test_model : public Coupled {
        public:
        helicopter_coupled_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
			shared_ptr<HelicopterCoupled> helicopterCoupled = addComponent<HelicopterCoupled>("helicopterCoupled", 1);
			
			std::string filepath1 = "../input_data/helicopter_coupled_inEvac_test" + testNumber + "_input.txt";
			std::string filepath2 = "../input_data/helicopter_coupled_inES_test" + testNumber + "_input.txt";
			
			auto helicopterCoupledInputEvac = addComponent<cadmium::lib::IEStream<EvacInfo>>("helicopterCoupledInputEvac",filepath1.c_str());
			auto helicopterCoupledInputES = addComponent<cadmium::lib::IEStream<HeloInfo>>("helicopterCoupledInputES",filepath2.c_str());
			
			addCoupling(helicopterCoupledInputEvac->out, helicopterCoupled->inEvac);
			addCoupling(helicopterCoupledInputES->out, helicopterCoupled->inES);
        }
    };
} // namespace cadmium::assignment1

#endif // __HELICOPTER_COUPLED_TEST_MODEL_HPP__
