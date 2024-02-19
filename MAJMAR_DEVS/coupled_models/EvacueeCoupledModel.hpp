#ifndef __EVACUEE_COUPLED_MODEL_HPP__
#define __EVACUEE_COUPLED_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <iostream>
#include <string>
#include <cassert>

// We include any models that are directly contained within this coupled model
#include "../atomic_models/evacuee.hpp"
#include "../atomic_models/filterLoc.hpp"

// data structures
#include "../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
    class EvacueeCoupled : public Coupled {
        public:
		
		Port<EvacInfo> in;
		Port<EvacInfo> outHelo;
		Port<EvacInfo> outFOL;
		Port<EvacInfo> outES;
		
        EvacueeCoupled(const std::string& id, int evacueeID, char triage_status): Coupled(id){


            // Declare and initialize all controller models (non-input/output)
			shared_ptr<Evacuee> evac = addComponent<Evacuee>("evacuee", evacueeID, triage_status);
			shared_ptr<FilterLoc> filterLoc = addComponent<FilterLoc>("filterLoc", evacueeID);
			
			// initialize coupled model ports
			in = addInPort<EvacInfo>("in");
			outHelo = addOutPort<EvacInfo>("outHelo");
			outFOL = addOutPort<EvacInfo>("outFOL");
			outES = addOutPort<EvacInfo>("outES");
			
            // Connect the atomic models with Internal Couplings
			addIC("filterLoc", "out", "evacuee", "in");
			
			// Connect the inputs of the coupled model with the inputs of some atomic model
			addEIC("in", "filterLoc", "in");
			
			// Connect the outputs of the coupled model with the outputs of some atomic model
			addEOC("evacuee", "outHelo", "outHelo");
			addEOC("evacuee", "outFOL", "outFOL");
			addEOC("evacuee", "outES", "outES");
        }
    };
} // namespace cadmium::assignment1

#endif // __EVACUEE_COUPLED_MODEL_HPP__
