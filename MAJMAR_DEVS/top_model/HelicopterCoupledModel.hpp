#ifndef __HELICOPTER_COUPLED_MODEL_HPP__
#define __HELICOPTER_COUPLED_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <iostream>
#include <string>
#include <cassert>

// We include any models that are directly contained within this coupled model
#include "../atomic_models/filterES.hpp"
#include "../atomic_models/filterEvac.hpp"
#include "../atomic_models/helicopter.hpp"

// data structures
#include "../data_structures/heloInfo.hpp"
#include "../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
    class HelicopterCoupled : public Coupled {
        public:
		
		Port<EvacInfo> inEvac;
		Port<HeloInfo> inES;
		Port<HeloInfo> outES;
		Port<EvacInfo> outEvac;
		
		
        HelicopterCoupled(const std::string& id, int helicopterID): Coupled(id){


             // Declare and initialize all controller models (non-input/output)
			shared_ptr<Helicopter> helo = addComponent<Helicopter>("helicopter", helicopterID);
			shared_ptr<FilterES> filterES = addComponent<FilterES>("filterES", helicopterID);
			shared_ptr<FilterEvac> filterEvac = addComponent<FilterEvac>("filterEvac", helicopterID);
			
			// initialize coupled model ports
			inEvac = addInPort<EvacInfo>("inEvac");
			inES = addInPort<HeloInfo>("inES");
			outES = addOutPort<HeloInfo>("outES");
			outEvac = addOutPort<EvacInfo>("outEvac");
			
            // Connect the atomic models with Internal Couplings
			addIC("filterES", "out", "helicopter", "inES");
			addIC("filterEvac", "out", "helicopter", "inEvac");
			
			// Connect the inputs of the coupled model with the inputs of some atomic model
			addEIC("inEvac", "filterEvac", "in");
			addEIC("inES", "filterES", "in");
			
			// Connect the outputs of the coupled model with the outputs of some atomic model
			addEOC("helicopter", "outES", "outES");
			addEOC("helicopter", "outEvac", "outEvac");
        }
    };
} // namespace cadmium::assignment1

#endif // __HELICOPTER_COUPLED_MODEL_HPP__
