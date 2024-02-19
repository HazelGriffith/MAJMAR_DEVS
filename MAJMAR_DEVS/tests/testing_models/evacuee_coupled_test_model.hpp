#ifndef __EVACUEE_COUPLED_TEST_MODEL_HPP__
#define __EVACUEE_COUPLED_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../coupled_models/EvacueeCoupledModel.hpp"

// data structures
#include "../../data_structures/heloInfo.hpp"
#include "../../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
    class evacuee_coupled_test_model : public Coupled {
        public:
        evacuee_coupled_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
			vector<EvacInfo> evacuees;
			int evacID = 1;
			for (int i = 0; i < 25; i++){
				evacuees.push_back(EvacInfo{evacID+i,-1,false,'W'});
				evacID++;
			}
			
			shared_ptr<EvacueeCoupled> evacueeCoupled = addComponent<EvacueeCoupled>("evacueeCoupled", 1, 'W');
			
			std::string filepath1 = "../../input_data/evacuee_coupled_in_test" + testNumber + "_input.txt";
			
			auto evacueeCoupledInput = addComponent<cadmium::lib::IEStream<EvacInfo>>("EvacueeCoupledInput",filepath1.c_str());
			
			addCoupling(evacueeCoupledInput->out, evacueeCoupled->in);
        }
    };
} // namespace cadmium::assignment1

#endif // __EVACUEE_COUPLED_TEST_MODEL_HPP__
