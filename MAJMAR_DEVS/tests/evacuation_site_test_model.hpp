#ifndef __EVACUATION_SITE_TEST_MODEL_HPP__
#define __EVACUATION_SITE_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>

// We include any models that are directly contained within this coupled model
#include "../atomic_models/evacuee.hpp"
#include "../atomic_models/evacueeManager.hpp"
#include "../atomic_models/filterES.hpp"
#include "../atomic_models/filterEvac.hpp"
#include "../atomic_models/filterLoc.hpp"
#include "../atomic_models/fol.hpp"
#include "../atomic_models/helicopter.hpp"
#include "../atomic_models/helicopterQueue.hpp"
#include "../atomic_models/helipadManager.hpp"

// data structures
#include "../data_structures/heloInfo.hpp"
#include "../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
    class evacuation_site_test_model : public Coupled {
        public:
        evacuation_site_test_model(const std::string& id): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
			vector<EvacInfo> evacuees;
			int evacID = 1;
			for (int i = 0; i < 25; i++){
				evacuees.push_back(EvacInfo{evacID+i,-1,false,'W'});
				evacID++;
			}
			
			shared_ptr<HelipadManager> helipadManager = addComponent<HelipadManager>("helipadManager", 15.0);
			shared_ptr<HelicopterQueue> helicopterQueue = addComponent<HelicopterQueue>("helicopterQueue");
			shared_ptr<EvacueeManager> evacueeManager = addComponent<EvacueeManager>("evacueeManager", evacuees);
			
			auto evacueeManagerInputEvac = addComponent<cadmium::lib::IEStream<EvacInfo>>("EvacueeManagerInputEvac","../input_data/evacuee_manager_inEvac_test_input.txt");
			
            // Declare and initialize all simulated input files (these must exist in the file system before compilation)

            // Connect the input files to the rest of the simulation with coupling
			
			addCoupling(helicopterQueue->outHM,helipadManager->inHQ);
			
			addCoupling(helipadManager->outHQ,helicopterQueue->inHM);
			addCoupling(helipadManager->outEM,evacueeManager->inHelo);
			
			addCoupling(evacueeManager->outHM,helipadManager->inEM);
			
			
			
        }
    };
} // namespace cadmium::assignment1

#endif // __EVACUATION_SITE_TEST_MODEL_HPP__
