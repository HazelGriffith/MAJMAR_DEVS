#ifndef __EVACUEE_MANAGER_TEST_MODEL_HPP__
#define __EVACUEE_MANAGER_TEST_MODEL_HPP__


#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// Models
#include "../../atomic_models/evacueeManager.hpp"

// Data structures
#include "../../data_structures/evacInfo.hpp"
#include "../../data_structures/heloInfo.hpp"

namespace cadmium::assignment1 {
    class evacuee_manager_test_model : public Coupled {
        public:
        evacuee_manager_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
			std::vector<EvacInfo> evacuees;
			evacuees.push_back(EvacInfo{1,-1,false,false,'G'});
			evacuees.push_back(EvacInfo{2,-1,false,false,'R'});
            auto evacueeManager = addComponent<EvacueeManager>("EvacueeManager", evacuees);

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
			std::string filepath1 = "../input_data/evacuee_manager_inEvac_test" + testNumber + "_input.txt";
			std::string filepath2 = "../input_data/evacuee_manager_inHM_test" + testNumber + "_input.txt";
			std::string filepath3 = "../input_data/evacuee_manager_inCGS_test" + testNumber + "_input.txt";
			
            auto evacueeManagerInputEvac = addComponent<cadmium::lib::IEStream<EvacInfo>>("EvacueeManagerInputEvac",filepath1.c_str());
			auto evacueeManagerInputHM = addComponent<cadmium::lib::IEStream<HeloInfo>>("EvacueeManagerInputHM",filepath2.c_str());
			auto evacueeManagerInputCGS = addComponent<cadmium::lib::IEStream<bool>>("EvacueeManagerInputCGS",filepath3.c_str());

            // Connect the input files to the rest of the simulation with coupling
			addCoupling(evacueeManagerInputEvac->out,evacueeManager->inEvac);
			addCoupling(evacueeManagerInputHM->out,evacueeManager->inHM);
			addCoupling(evacueeManagerInputCGS->out,evacueeManager->inCGS);
        }
    };
} // namespace cadmium::assignment1

#endif // __EVACUEE_MANAGER_TEST_MODEL_HPP__
