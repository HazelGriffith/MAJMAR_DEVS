#ifndef __EVACUATION_SITE_MODEL_HPP__
#define __EVACUATION_SITE_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <iostream>
#include <string>
#include <cassert>

// We include any models that are directly contained within this coupled model
#include "../atomic_models/evacueeManager.hpp"
#include "../atomic_models/helicopterQueue.hpp"
#include "../atomic_models/helipadManager.hpp"

// data structures
#include "../data_structures/heloInfo.hpp"
#include "../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
    class EvacuationSite : public Coupled {
        public:
		
		Port<EvacInfo> inEvac;
		Port<HeloInfo> inHelo;
		Port<HeloInfo> outHelo;
		Port<EvacInfo> outEvac;
		
        EvacuationSite(const std::string& id, double timeToLoad, vector<EvacInfo> evacuees): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
			shared_ptr<HelicopterQueue> hq = addComponent<HelicopterQueue>("helicopterQueue");
			shared_ptr<HelipadManager> hm = addComponent<HelipadManager>("helipadManager", timeToLoad);
			shared_ptr<EvacueeManager> em = addComponent<EvacueeManager>("evacueeManager", evacuees);
			
			// initialize coupled model ports
			inEvac = addInPort<EvacInfo>("inEvac");
			inHelo = addInPort<HeloInfo>("inHelo");
			outHelo = addOutPort<HeloInfo>("outHelo");
			outEvac = addOutPort<EvacInfo>("outEvac");
			
            // Connect the atomic models with Internal Couplings
			addIC("helicopterQueue", "outHM", "helipadManager", "inHQ");
			addIC("helipadManager", "outHQ", "helicopterQueue", "inHM");
			addIC("helipadManager", "outEM", "evacueeManager", "inHM");
			addIC("evacueeManager", "outHM", "helipadManager", "inEM");
			
			// Connect the inputs of the coupled model with the inputs of some atomic model
			addEIC("inEvac", "evacueeManager", "inEvac");
			addEIC("inHelo", "helicopterQueue", "inHelo");
			
			// Connect the outputs of the coupled model with the outputs of some atomic model
			addEOC("evacueeManager", "outEvac", "outEvac");
			addEOC("helicopterQueue", "outHelo", "outHelo");
			addEOC("helipadManager", "outHelo", "outHelo");			
        }
    };
} // namespace cadmium::assignment1

#endif // __EVACUATION_SITE_MODEL_HPP__
