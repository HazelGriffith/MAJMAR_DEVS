#ifndef __EVACUEE_MANAGER_TEST_MODEL_HPP__
#define __EVACUEE_MANAGER_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../atomic_models/evacueeManager.hpp"
#include "../../data_structures/evacInfo.hpp"
#include "../../data_structures/heloInfo.hpp"

namespace cadmium::assignment1 {
    class evacuee_manager_test_model : public Coupled {
        public:
        evacuee_manager_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
			std::vector<EvacInfo> evacuees;
			for (int i = 1; i <= 25; i++){
				evacuees.push_back(EvacInfo{i, -1, false, 'G'});
			}
			for (int i = 26; i <= 51; i++){
				evacuees.push_back(EvacInfo{i, -1, false, 'W'});
			}
			for (int i = 52; i <= 77; i++){
				evacuees.push_back(EvacInfo{i, -1, false, 'Y'});
			}
			for (int i = 78; i < 103; i++){
				evacuees.push_back(EvacInfo{i, -1, false, 'R'});
			}
            auto evacueeManager = addComponent<EvacueeManager>("EvacueeManager", evacuees);

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
			std::string filepath1 = "../../input_data/evacuee_manager_inEvac_test" + testNumber + "_input.txt";
			std::string filepath2 = "../../input_data/evacuee_manager_inHM_test" + testNumber + "_input.txt";
			
			
            auto evacueeManagerInputEvac = addComponent<cadmium::lib::IEStream<EvacInfo>>("EvacueeManagerInputEvac",filepath1.c_str());
			auto evacueeManagerInputHM = addComponent<cadmium::lib::IEStream<HeloInfo>>("EvacueeManagerInputHM",filepath2.c_str());

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(evacueeManagerInputEvac->out,evacueeManager->inEvac);
			addCoupling(evacueeManagerInputHM->out,evacueeManager->inHM);
        }
    };
} // namespace cadmium::assignment1

#endif // __EVACUEE_MANAGER_TEST_MODEL_HPP__
